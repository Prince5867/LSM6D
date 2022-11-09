#include <rtthread.h>
#include <rtdevice.h>
#include "Pin.h"
/*按键中断引脚设置*/
#define PWM_INC_PIN          rt_pin_get("PE.4")
#define PWM_DEC_PIN          rt_pin_get("PE.5")
#define PWM_STOP_PIN         rt_pin_get("PB.0")
#define PWM_REVS_PIN         rt_pin_get("PH.3")
#define PWM_IN1_PIN         rt_pin_get("PE.2")
#define PWM_IN2_PIN         rt_pin_get("PE.6")

/*pwm 设置*/
#define PWM_DEV_NAME        "pwm5"
#define PWM_DEV_CHANNEL     1
struct rt_device_pwm *pwm_dev;
/*PWM参数设置*/
rt_uint32_t period = 500000;
int pulse=150000;
unsigned int  flag=0;
void pwm_inc_callback(void *args)
{

    pulse+=10000;
   if(pulse>period)
    {
        pulse = 200000;
    }
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
}
void pwm_dec_callback(void *args)
{
    pulse-=10000;
    if(pulse<0)
    {
        pulse = 0;

    }
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
}
void pwm_revs_callback(void *args)
{
    if(rt_pin_read(PWM_IN1_PIN)==PIN_HIGH)
    {
        rt_pin_write(PWM_IN2_PIN, PIN_HIGH);
        rt_pin_write(PWM_IN1_PIN, PIN_LOW);
    }

    else{
        rt_pin_write(PWM_IN1_PIN, PIN_HIGH);
        rt_pin_write(PWM_IN2_PIN, PIN_LOW);
    }
}
void pwm_stop_callback(void *args)
{

    if(flag==0){
        rt_pin_write(PWM_IN1_PIN, PIN_LOW);
        rt_pin_write(PWM_IN2_PIN, PIN_LOW);
        flag=1;
    }
    else{
        rt_pin_write(PWM_IN2_PIN, PIN_HIGH);
        rt_pin_write(PWM_IN1_PIN, PIN_LOW);
        flag=0;
    }

}
void pwm_control()
{

    /* PWM占空比增加引脚 */
    rt_pin_mode(PWM_INC_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(PWM_INC_PIN, PIN_IRQ_MODE_FALLING, pwm_inc_callback, RT_NULL);
    rt_pin_irq_enable(PWM_INC_PIN, PIN_IRQ_ENABLE);

     //pwm减少占空比引脚
    rt_pin_mode(PWM_DEC_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(PWM_DEC_PIN, PIN_IRQ_MODE_FALLING, pwm_dec_callback, RT_NULL);
    rt_pin_irq_enable(PWM_DEC_PIN, PIN_IRQ_ENABLE);

    // pwm停止引脚
    rt_pin_mode(PWM_STOP_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(PWM_STOP_PIN, PIN_IRQ_MODE_FALLING,pwm_stop_callback, RT_NULL);
    rt_pin_irq_enable(PWM_STOP_PIN, PIN_IRQ_ENABLE);
    //电机反转引脚

    rt_pin_mode(PWM_REVS_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(PWM_REVS_PIN, PIN_IRQ_MODE_FALLING,pwm_revs_callback, RT_NULL);
    rt_pin_irq_enable(PWM_REVS_PIN, PIN_IRQ_ENABLE);
    //IN1 IN2引脚设置
    rt_pin_mode(PWM_IN1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(PWM_IN2_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(PWM_IN1_PIN, PIN_HIGH);
    rt_pin_write(PWM_IN2_PIN, PIN_LOW);


    /*pwm初始化*/

    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm device can't find!\n");
    }
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

}

MSH_CMD_EXPORT(pwm_control,pwm control);
