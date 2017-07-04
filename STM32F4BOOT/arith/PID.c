//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2012-05-20  
 *
 *            修改时间: 2012-05-20
 *            修改原因:
 *
 *                     
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
#include "mystd.h"  
#include "m_config.h"
#include "Pid.h"

//================================================================================================
PIDREG   PidV;
//================================================================================================

//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void   InitPIDParam(PIDREG *v)
{
    v->Kp = 0.38;
    v->Ki = 0.02;
 //   v->Kd = 0.8;
    v->OutMax = 200;
    v->OutMin = -10;
    v->Ref    = 500.0;
}


//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void  SetPIDRefV(PIDREG *pid, UINT32 v)
{
    pid->Ref    = v;
}



//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void PIDArith(PIDREG *v)
{	 // Compute the error
    v->Err = -(v->Fdb - v->Ref);
    
    
    // Compute the proportional output
    v->Up = v->Kp *v->Err;
    
    // Compute the integral output 
    v->Ui = v->Ui + v->Ki * v->Err;// + _IQmpy(v->Kc,v->SatErr);
    
    // Compute the derivative output
    /////  v->Ud = v->Kd *(v->Up- v->Up1);   
    
    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;
    
    // Saturate the output
    if (v->OutPreSat > v->OutMax)
    {
        v->Out =  v->OutMax;
    }
    else if (v->OutPreSat < v->OutMin)
    {
        v->Out =  v->OutMin;
    }
    else
    {
        v->Out = v->OutPreSat;
    }
    // Compute the saturate difference
    v->SatErr = v->Out - v->OutPreSat;
    
    // Update the previous proportional output
    v->Up1 = v->Up; 

}




/*
void pid_reg3_calc(PIDREG3 *v)
{	
    // Compute the error
    v->Err =-( v->Ref - v->Fdb);

    // Compute the proportional output
    v->Up = v->Kp *v->Err;
  
    // Compute the integral output 
    v->Ui = v->Ui + v->Ki *v->Err;// + _IQmpy(v->Kc,v->SatErr);

    // Compute the derivative output
    v->Ud = v->Kd *(v->Up- v->Up1);   
    
    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;

    // Saturate the output
    if (v->OutPreSat > v->OutMax)
      v->Out =  v->OutMax;
    else if (v->OutPreSat < v->OutMin)
      v->Out =  v->OutMin;
    else
      v->Out = v->OutPreSat;

    // Compute the saturate difference
    v->SatErr = v->Out - v->OutPreSat;

    // Update the previous proportional output
    v->Up1 = v->Up; 

}

*/
