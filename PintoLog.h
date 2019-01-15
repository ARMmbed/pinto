#ifndef __PINTOLOG_H__
#define __PINTOLOG_H__

#include "MbedCloudClient.h"
#include "CircularBufferFile.h"
#include "DataPath.h"
#include "ControlPlane.h"
#include "ConsoleSingleton.h"

#define _CLOUD_LOG(buffer, buf_size, ...) \
    snprintf(buffer, buf_size, __VA_ARGS__); \
    write_maybe_to_cloud(buffer)


template <class WriteObject>
class PintoLogger {
    private:
        PintoLogger() : ready(false) {
        }

    public:
        PintoLogger(MbedCloudClient*const cloudClient): ready(false), cloudClient(cloudClient), controlPlane(cloudClient) {
            //Hook in data path to console retarget
            //console.observe(&dataPath);
            ConsoleSingleton::Instance().observe(&dataPath);
            
            //retarget console is handled by implrementing mbed_override_console in the global space

        } 
        void set_ready(bool ready) { 
            if (ready && !this.ready){
                //push_any_faults
            }
            this.ready = ready; 
        }
        bool is_ready() const { return ready; }

    private:
        bool ready;
        MbedCloudClient*const cloudClient;
        //ConsoleSingleton console;
        DataPath<WriteObject> dataPath;
        ControlPlane controlPlane;
};

// TODO turn these into sprintfs and write to cloud directly
//void push_any_faults(char* buffer, int buf_size) {
//    if (!mbed_fault_occurred)
//        return;
//
//    //This doesnt work in single thread
//    pc.printf("Mbed fault occurred. postponing serial until after cloud registers\n");
//    while(!client->is_client_registered()) {
//        wait(0.5);
//    }
//    if(error_ctx.error_status < 0) {
//        printf("\nGot Reboot Info\n");
//        _CLOUD_LOG(buffer, buf_size, "Mbed error reboot occurred\n");
//        _CLOUD_LOG(buffer, buf_size, "e_status:[%lx]\ne_addr:[%lx]\ne_val:[%lx]\n", error_ctx.error_status, error_ctx.error_address, error_ctx.error_value);
//        _CLOUD_LOG(buffer, buf_size, "t_id:[%lx]\nt_entry_addr:[%lx]\nt_stack_sz:[%lx]\n", error_ctx.thread_id, error_ctx.thread_entry_address, error_ctx.thread_stack_size);
//        _CLOUD_LOG(buffer, buf_size, "t_stack_mem:[%lx]\nt_curr_sp:[%lx]\ne_rebt_cnt:[%lx]\n", error_ctx.thread_stack_mem, error_ctx.thread_current_sp, error_ctx.error_reboot_count);
//        write_maybe_to_cloud(error_ctx.error_filename);
//        _CLOUD_LOG(buffer, buf_size, "e_lineno:[%lx]\n", error_ctx.error_line_number);
//
//        if(error_ctx.error_status == MBED_ERROR_HARDFAULT_EXCEPTION) {
//            mbed_get_reboot_fault_context(&fault_ctx);
//            _CLOUD_LOG(buffer, buf_size, "R0:[%lx]\nR1:[%lx]\nR2:[%lx]\nR3:[%lx]\n", fault_ctx.R0_reg, fault_ctx.R1_reg, fault_ctx.R2_reg, fault_ctx.R3_reg);
//            _CLOUD_LOG(buffer, buf_size, "R4:[%lx]\nR5:[%lx]\nR6:[%lx]\nR7:[%lx]\n", fault_ctx.R4_reg, fault_ctx.R5_reg, fault_ctx.R6_reg, fault_ctx.R7_reg);
//            _CLOUD_LOG(buffer, buf_size, "R8:[%lx]\nR9:[%lx]\nR10:[%lx]\nR11:[%lx]\n", fault_ctx.R8_reg, fault_ctx.R9_reg, fault_ctx.R10_reg, fault_ctx.R11_reg);
//            _CLOUD_LOG(buffer, buf_size, "R12:[%lx]\nSP:[%lx]\nLR:[%lx]\nPC:[%lx]\n", fault_ctx.R12_reg, fault_ctx.SP_reg, fault_ctx.LR_reg, fault_ctx.PC_reg);
//            _CLOUD_LOG(buffer, buf_size, "xPSR:[%lx]\nPSP:[%lx]\nMSP:[%lx]\n", fault_ctx.xPSR, fault_ctx.PSP, fault_ctx.MSP);
//            _CLOUD_LOG(buffer, buf_size, "EXC_RETURN:[%lx]\nCONTROL:[%lx]\n", fault_ctx.EXC_RETURN, fault_ctx.CONTROL);
//        }
//    }
//    mbed_reset_reboot_error_info();
//}
#endif /*__PINTOLOG_H__*/
