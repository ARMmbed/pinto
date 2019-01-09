#ifndef __PINTOLOG_H__
#define __PINTOLOG_H__

#define _CLOUD_LOG(buffer, buf_size, ...) \
    snprintf(buffer, buf_size, __VA_ARGS__); \
    write_maybe_to_cloud(buffer)

template <class WriteObject> Writer {
    public:
        Writer(WriteObject* writer) : writer(writer) {}
        size_t write(void* data, size_t len) { return writer->write(data, len); } // Default

    private:
        WriteObject* writer;
};

template <> Writer<M2MResource> {
    public:
        Writer(M2MResource* writer) : writer(writer) {}
        size_t write(void* data, size_t len) { return writer->set_value(data, len);}

    private:
        M2MResource* writer;

};

// DataPathWriteObject must has a ->write call
// This class is only outbound to network
template <class WriteObject, template<class> DataPathWriteObject=Writer> DataPath {
    public:
        DataPath(DataPathWriteObject<WriteObject>* writer): writer(writer) {}
        // Would make this const, but cloud client doesnt like const correctness
        size_t write(void * data, size_t len);

    private:
        DataPathWriteObject<WriteObject> *const writer;
};

template <class WriteObject>
size_t DataPath<WriteObject>::write(void * data, size_t len) 
{ 
    // Dont over trace
    mbed_trace_config_set(TRACE_ACTIVE_LEVEL_NONE);
    writer->write((uint8_t*)local_log_buffer.data(), (uint32_t)local_log_buffer.size());
    mbed_trace_config_set(trace_mode);
}

class DataNexusBaseInterface {
    //public:

};

template <class WriteObject>
class PintoLogger {
    private:
        PintoLogger() : ready(false) {}
    public:
        PintoLogger(SimpleM2MClient*const cloudClient): ready(false), cloudClient(cloudClient) {} 
        void set_ready(bool ready) { this.ready = ready; }
        bool is_ready() const { return ready; }

    private:
        bool ready;
        SimpleM2MClient*const cloudClient;
};

// TODO turn these into sprintfs and write to cloud directly
void push_any_faults(char* buffer, int buf_size) {
    if (!mbed_fault_occurred)
        return;

    //This doesnt work in single thread
    pc.printf("Mbed fault occurred. postponing serial until after cloud registers\n");
    while(!client->is_client_registered()) {
        wait(0.5);
    }
    if(error_ctx.error_status < 0) {
        printf("\nGot Reboot Info\n");
        _CLOUD_LOG(buffer, buf_size, "Mbed error reboot occurred\n");
        _CLOUD_LOG(buffer, buf_size, "e_status:[%lx]\ne_addr:[%lx]\ne_val:[%lx]\n", error_ctx.error_status, error_ctx.error_address, error_ctx.error_value);
        _CLOUD_LOG(buffer, buf_size, "t_id:[%lx]\nt_entry_addr:[%lx]\nt_stack_sz:[%lx]\n", error_ctx.thread_id, error_ctx.thread_entry_address, error_ctx.thread_stack_size);
        _CLOUD_LOG(buffer, buf_size, "t_stack_mem:[%lx]\nt_curr_sp:[%lx]\ne_rebt_cnt:[%lx]\n", error_ctx.thread_stack_mem, error_ctx.thread_current_sp, error_ctx.error_reboot_count);
        write_maybe_to_cloud(error_ctx.error_filename);
        _CLOUD_LOG(buffer, buf_size, "e_lineno:[%lx]\n", error_ctx.error_line_number);

        if(error_ctx.error_status == MBED_ERROR_HARDFAULT_EXCEPTION) {
            mbed_get_reboot_fault_context(&fault_ctx);
            _CLOUD_LOG(buffer, buf_size, "R0:[%lx]\nR1:[%lx]\nR2:[%lx]\nR3:[%lx]\n", fault_ctx.R0_reg, fault_ctx.R1_reg, fault_ctx.R2_reg, fault_ctx.R3_reg);
            _CLOUD_LOG(buffer, buf_size, "R4:[%lx]\nR5:[%lx]\nR6:[%lx]\nR7:[%lx]\n", fault_ctx.R4_reg, fault_ctx.R5_reg, fault_ctx.R6_reg, fault_ctx.R7_reg);
            _CLOUD_LOG(buffer, buf_size, "R8:[%lx]\nR9:[%lx]\nR10:[%lx]\nR11:[%lx]\n", fault_ctx.R8_reg, fault_ctx.R9_reg, fault_ctx.R10_reg, fault_ctx.R11_reg);
            _CLOUD_LOG(buffer, buf_size, "R12:[%lx]\nSP:[%lx]\nLR:[%lx]\nPC:[%lx]\n", fault_ctx.R12_reg, fault_ctx.SP_reg, fault_ctx.LR_reg, fault_ctx.PC_reg);
            _CLOUD_LOG(buffer, buf_size, "xPSR:[%lx]\nPSP:[%lx]\nMSP:[%lx]\n", fault_ctx.xPSR, fault_ctx.PSP, fault_ctx.MSP);
            _CLOUD_LOG(buffer, buf_size, "EXC_RETURN:[%lx]\nCONTROL:[%lx]\n", fault_ctx.EXC_RETURN, fault_ctx.CONTROL);
        }
    }
    mbed_reset_reboot_error_info();
}
#endif /*__PINTOLOG_H__*/
