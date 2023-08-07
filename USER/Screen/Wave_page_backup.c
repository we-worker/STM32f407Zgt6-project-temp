
////////////////////////////////////////////////////////////////////////////////

/*

// 数字滤波器

//								di.k=2*PI*9000/Fs;
//				di.lVal=0;
//        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC1_Value[i]);
//					ADC1_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC1_Value[i]);
//					ADC1_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC1_Value[i]);
//					ADC1_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC1_Value[i]);
//					ADC1_Value[i]=_constrain( temp ,0,4095);
//        }        for (int i = 0; i < FFT_LENGTH; i++)
//        {
//					float temp=rcLfFiter(&di,ADC1_Value[i]);
//					ADC1_Value[i]=_constrain( temp ,0,4095);
//        }
//

void auto_change(void)
{
    char display_str[30];
    //======================自动缩放波形======================
    int max_indx = fft_max_index();
    max_freq = (fft_freq(max_indx, 1) + max_freq) / 2;
    adc_show_size = Fs / max_freq * 5;

    adc_show_size = _constrain(adc_show_size, 5, 1024);
    //======================自动缩放FFT_FS======================

    //    if (Frequency > 120e3) // 高频使用等效采用
    //    {
    //        uint32_t test_freq = ((uint32_t)(1.0f * Frequency / 1e3 + 0.5)) * 1e3; // 这里没有完成，需要知道信号的准确频率才行
    //        int fenpin = Frequency / 10e5 + 1;                                     // 定时器频率与信号频率的倍数。
    //        uint32_t Fs_tim = test_freq / fenpin;
    //        // int delay=0;//自动控制延迟时间
    //        int delay = 250e3 / Frequency + 1;
    //        // double Fs_tim_true = 14e6 / ((14e6 / Fs_tim) + delay);
    //        // Fs = (uint32_t)(1.0f * test_freq / (test_freq - fenpin * Fs_tim_true) * Fs_tim_true + 0.5);
    //        Fs = 1.0f * Frequency / max_freq * Fs; // 也不根据采集的波形计算了，直接让fft解算的最大频率值等于频率计的。

    //        TIM2_Init2((u16)(42e6 / Fs_tim) - 1 + delay, 1);                                 // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
    //        sprintf((char *)display_str, "Tim2:%d    ", ((u16)(14e6 / Fs_tim) - 1 + delay)); // 1024/2
    //        LCD_DisplayString(70, 268, 12, display_str);                                     // 实际电压数值
    //    }
    //    else
    //    {
    //        // 需要10k下100hz分辨率时 Fs_temp = (int)(max_freq / 1000 / 2 + 1) * 1 * 7e3;
    //       uint32_t Fs_tem	p = (int)(max_freq / 1000 / 2 + 1) * 2 * 7e3;
    //       if (fabs(Fs_temp / (2 * 7e3) - Fs / (2 * 7e3)) >= 3) // 防止来回疯狂变化。
    //       {
    //           Fs = _constrain(Fs_temp, 1e3, 7e5);
    //           // Fs=30000;
    //           TIM2_Init2((u16)(14e6 / Fs) - 1, 5); // 定时器2时钟84M，分频系数84，84M/6=14000K 所以499次为28k
    //       }
    //       sprintf((char *)display_str, "Tim2:%d    ", (u16)(14e6 / Fs) - 1); // 1024/2
    //       LCD_DisplayString(70, 268, 12, display_str);                       // 实际电压数值
    //    }
}

void DDS_out_backup(void)
{
    int fft_tesst_num = 30;
    char display_str[50];

    f_value[f_value_index++] = fft_value(feed_back_freq[0]);

    if (freq_change_count == 1)
    {
        //         int num = 0;
        //         int nums[5] = {0};

        //         for (int j = 1; j < 5; j++)
        //         {
        //             set_freq = sum_min_freqs[j][1] ;
        // 					AD9833_WaveSeting(set_freq, 0, SIN_WAVE, 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
        //    sprintf((char *)display_str, "set_freq:%.2f", set_freq);               // 1024/2
        //     LCD_DisplayString(120, 50, 12, display_str);                           // 实际电压数值
        //             f_value_index = 0;

        //             for (int k = 0; k < fft_tesst_num; k++)
        //             {

        //                 // 高速时需要开关tim2实现采集，不卡顿
        //                 if (TIM2->CR1 == 0)        // 等待被关闭  说明FTT计算完毕
        //                     TIM_Cmd(TIM2, ENABLE); // 使能定时器2
        //                 while (TIM2->CR1 != 0)
        //                 {
        //                 } // 等待被关闭，说明采样完毕

        //                 FFT(ADC2_Value);
        //                 f_value[f_value_index++] = fft_value(feed_back_freq[0]);
        //             }
        //             f_value_index = 0;

        //             for (int i = 1; i < fft_tesst_num; i++)
        //             {
        //                 f_value_sum += fabs(f_value[i] - f_value[i - 1]);
        //             }
        //             if (f_value_sum > 130)
        //             {
        //                 num++;
        //                 sum_min_freqs[j][0] = f_value_sum;
        //                 nums[j] = 1;
        //             }
        //             if (num >= 4)
        //             {
        //                 break;
        //             }
        //         }
        //         for (int j = 0; j < 5; j++)
        //         {
        //             if (nums[j] == 0)
        //             {
        //                 set_freq = sum_min_freqs[j][1];
        //             }
        //         }
        // set_freq = sum_min_freq;
        AD9833_WaveSeting(set_freq, 0, base_freq[0][1], 0);      // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
        sprintf((char *)display_str, "set_freq:%.2f", set_freq); // 1024/2
        LCD_DisplayString(120, 50, 12, display_str);             // 实际电压数值
        freq_change_count++;
    }
    if (f_value_index >= fft_tesst_num && freq_change_count < 1)
    {

        f_value_index = 0;

        f_value_sum = 0;
        for (int i = 1; i < fft_tesst_num; i++)
        {
            f_value_sum += fabs(f_value[i] - f_value[i - 1]);
        }

        // error[0]=error[1];
        // error[1]=f_value_sum;
        // set_freq=f_GradDesO(error,set_freqs);
        // set_freqs[0]=set_freqs[1];
        // set_freqs[1]=set_freq;
        // AD9833_WaveSeting(set_freq, 0, base_freq[0][1], 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ

        // sprintf((char *)display_str, "%.2f,%d,%.2f,%.2f\n", set_freq,error[1],fft_value(feed_back_freq[0]),set_freqs[1]-set_freqs[0]); // 1024/2
        // USART_OUT(USART6, (uint8_t *)display_str);

        for (int j = 0; j < 5; j++)
        {
            if (sum_min_freqs[j][0] > f_value_sum)
            {
                for (int i = j; i < 4; i++)
                {
                    sum_min_freqs[i + 1][0] = sum_min_freqs[i][0];
                    sum_min_freqs[i + 1][1] = sum_min_freqs[i][1];
                }
                sum_min_freqs[j][1] = set_freq - 0.1;
                sum_min_freqs[j][0] = f_value_sum;
                break;
            }
        }
        if (f_value_sum_min > f_value_sum && f_value_sum > 0)
        {
            f_value_sum_min = f_value_sum;
            sum_min_freq = set_freq - 0.1;
        }

        //        if (f_value_sum > 3 * f_value_sum_min && set_freq > sum_min_freq + 0.2)
        //        {
        //            set_freq = sum_min_freq - 0.2;
        //            freq_change_count++;
        //        }
        else if (f_value_sum > f_value_sum_min / 2)
        {
            set_freq += 0.1;
            AD9833_WaveSeting(set_freq, 0, base_freq[0][1], 0); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
            // delay_ms(1);
        }

        if (set_freq >= base_freq[0][0] + 0.3)
        {
            set_freq = sum_min_freq - 0.1;
            freq_change_count++;
        }
    }
    else
    {

        // // 相位控制
        if (freq_change_count >= 2)
        {
            float nowpeak = ADC_peak2peak(ADC2_Value);
            // nowpeak = (nowpeak + peak2peak_last) / 2;
            // 2000
            //    if(nowpeak>fft_show_idx+2000) {
            //        phase_1=phase_1-50; //120
            //    } else if(nowpeak< fft_show_idx+2000) {
            //        phase_1=phase_1+50;//115
            //    }
            //    else {
            //         phase_1=phase_1;
            //    }
            // phase_1 = phase_1 + 5*(nowpeak-peak2peak_last);
            // Perror[0] = Perror[1];
            // Perror[1] = nowpeak;
            // nowPhaseis[0] = nowPhaseis[1];
            // nowPhaseis[1] = phase_1;
            // if (fft_show_idx+goal_phase > 1800){
            //     int deta = Follow_PID(&phase_pid, fft_show_idx+goal_phase-nowpeak,1);
            //     phase_1 = phase_1 + deta;
            // }
            // else{

            // 该部分为自动归零移相
            //  if (fft_show_idx+goal_phase >1200 && stop_move){
            //      fft_show_idx -=phase_step;
            //  }
            //  else{
            //      stop_move = 0;
            //  }

            if(nowpeak < lowest_phase)
            {
                lowest_phase = nowpeak;
            }
            if (fft_show_idx+goal_phase<=lowest_phase+lowphase_add){
                set_phase = lowest_phase+lowphase_add;
            }
            else{
                set_phase = fft_show_idx+goal_phase;
            }
            if(set_phase == lowest_phase+lowphase_add){
                if(nowpeak - set_phase > 500){
                    lower_count++;
                    // lowphase_add++;
                }
                else{
                    lower_count = 0;
                }
                if(lower_count >3){
                    lower_count = 0;
                    lowphase_add++;
                }

            }

            if (phase_ChangeD(Perror, nowPhaseis) == 1)
            {
                //    if(nowpeak>fft_show_idx+2000) {
                //        phase_1=phase_1-move_step; //120
                //    } else if(nowpeak< fft_show_idx+2000) {
                //        phase_1=phase_1+move_step;//115
                //    }
                //    else {
                //         phase_1=phase_1;
                //    }

                int deta = Follow_PID(&phase_pid, set_peak1 - nowpeak, 1);
                phase_1 = phase_1 + deta;
            }
            else if (phase_ChangeD(Perror, nowPhaseis) == 0)
            {
                // if(nowpeak>fft_show_idx+2000) {
                //     phase_1=phase_1+move_step; //120
                // } else if(nowpeak< fft_show_idx+2000) {
                //     phase_1=phase_1-move_step;//115
                // }
                // else {
                //         phase_1=phase_1;
                // }
                int deta = Follow_PID(&phase_pid, set_peak1 - nowpeak, 1);
                phase_1 = phase_1 - deta;
            }
            // else if (phase_ChangeD(Perror,nowPhaseis) == 1){
            //        if(nowpeak>=fft_show_idx+2000) {
            //            phase_1=phase_1-move_step; //120
            //        } else if(nowpeak< fft_show_idx+2000) {
            //            phase_1=phase_1+move_step;//115
            //        }
            // }
            // else if (phase_ChangeD(Perror,nowPhaseis) == 0){
            //         if(nowpeak>=fft_show_idx+2000) {
            //             phase_1=phase_1+move_step; //120
            //         } else if(nowpeak< fft_show_idx+2000) {
            //             phase_1=phase_1-move_step;//115
            //         }
            // }
            // }
            // if (phase_ChangeD(Perror,nowPhaseis) == 1){
            //            if(nowpeak>fft_show_idx+2000) {
            //                phase_1=phase_1-move_step; //120
            //            } else if(nowpeak< fft_show_idx+2000) {
            //                phase_1=phase_1+move_step;//115
            //            }
            //            else {
            //                 phase_1=phase_1;
            //            }

            //         // int deta = Follow_PID(&phase_pid, fft_show_idx+goal_phase-nowpeak,1);
            //         // phase_1 = phase_1 + deta;
            // }
            // else{
            //            if(nowpeak>fft_show_idx+2000) {
            //                phase_1=phase_1+move_step; //120
            //            } else if(nowpeak< fft_show_idx+2000) {
            //                phase_1=phase_1-move_step;//115
            //            }
            //            else {
            //                 phase_1=phase_1;
            //            }
            //         // int deta = Follow_PID(&phase_pid, fft_show_idx+goal_phase-nowpeak,0);
            //         // phase_1 = phase_1 + deta;
            // }

            // sprintf((char *)display_str, "%.2f,%.2f,%.2f,%d\n", fft_value(feed_back_freq[0]), nowpeak, peak2peak_last, phase_1); // 1024/2
            // USART_OUT(USART6, (uint8_t *)display_str);
            // sprintf((char *)display_str, "%.2f,%.2f,%.2f,%d\n", phase_pid.error_acc, nowpeak, peak2peak_last, phase_1); // 1024/2
            // USART_OUT(USART6, (uint8_t *)display_str);
            peak2peak_last = nowpeak;
            // // phase_1=phase_1+FOLLOW;
            while (phase_1 >= 4095)
                phase_1 -= 4095;
            while (phase_1 < 0)
                phase_1 += 4095;
            SetAD9833PhaseRegister(phase_1); // 2KHz,	频率寄存器0，正弦波输出 ,初相位0			//1HZ
                                             // delay_ms(100);
            // AD9833_WaveSeting(set_freq,0,SIN_WAVE,0);

            // phase_count++;
            // if(phase_count>=10){
            //     phase_count=0;
            //     if(nowpeak<min_peak2peak*1.5){
            //         min_peak2peak=nowpeak;
            //         goal_phase-=100;
            //     }else if(nowpeak>min_peak2peak){
            //          goal_phase+=1000;
            //     }
            // }
        }
    }
    if (f_value_index >= fft_tesst_num)
        f_value_index = 0;

    // 屏幕一开就会有大杂波

    // // sprintf((char *)display_str, "deta_f_value0:%d\t", deta_f_value[0]); // 1024/2
    // // LCD_DisplayString(120, 86, 12, display_str);                         // 实际电压数值
    // // deta_f_value[0] = deta_f_value[1];
    // BACK_COLOR = 0x9f31;                                                   // 背景色
    // sprintf((char *)display_str, "set_freq:%.2f", set_freq);               // 1024/2
    // LCD_DisplayString(120, 50, 12, display_str);                           // 实际电压数值
    // sprintf((char *)display_str, "f_value_sum:%d  ", f_value_sum);         // 1024/2
    // LCD_DisplayString(120, 62, 12, display_str);                           // 实际电压数值
    // sprintf((char *)display_str, "f_value_sum_min:%d  ", f_value_sum_min); // 1024/2
    // LCD_DisplayString(120, 74, 12, display_str);                           // 实际电压数值
    // sprintf((char *)display_str, "sum_min_freq:%.2f  ", sum_min_freq);     // 1024/2
    // LCD_DisplayString(100, 86, 12, display_str);                           // 实际电压数值
    // // sprintf((char *)display_str, "deta_f_value1:%d", deta_f_value[1]); // 1024/2
    // // LCD_DisplayString(120, 74, 12, display_str);                       // 实际电压数值
}
*/