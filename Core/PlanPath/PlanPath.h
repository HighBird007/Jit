#ifndef PlanPath_h
#define PlanPath_h
#include "GPS.h"
#include "math.h"
#include "SteeringServo.h"
#include "JY901S.h"
//到达目标经纬度的误差距离 deviation误差
#define deviationDistance 2

//路线的标记经纬度
typedef struct{
	double Longitude;
	double Latitude;
}PathMarking;


//船体行驶的目标点
extern PathMarking nextPlanMarking;

//写好存放的路线规划
void planMarkingInit(void);

//根据当前的gps数据 更新下一个路线规划
void updatePlanMarking(void);

//通过当前经纬度 和 目标经纬度    
/*计算出当前位置 和目标位置的距离*/
double calculateDistance(void);

//通过当前经纬度 和 目标经纬度    
/*计算出当前位置 和目标位置的的朝向*/
double calculateBearing(void);

extern Pose curPose;

#endif
