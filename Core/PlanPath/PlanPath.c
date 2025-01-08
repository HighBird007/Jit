#include "PlanPath.h"

PathMarking pm[20];

PathMarking nextPlanMarking;

// 将角度转换为弧度 这个是辅助计算 无需调用
double degrees_to_radians(double degrees);

//需要前往下一个标记的经纬度
uint32_t curPlanMarkingFlag = 0;

//存放标记点数量!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
uint32_t planMarkingCount = 4 ;

//预设好的经纬度路线规划 预设好之后 需要修改上面的存放数
void planMarkingInit(void){
	
//这个是坤泰白桥最近的那个大鱼池 四个点
/*
pm[0].Longitude =  118.961030;
pm[0].Latitude = 31.807128;


pm[1].Longitude = 118.961008;
pm[1].Latitude = 31.807380;

pm[2].Longitude = 118.961295;
pm[2].Latitude = 31.807392;
	
pm[3].Longitude = 118.961302;
pm[3].Latitude = 31.807157;
*/
	
//下面是金陵科技学院科技楼下的花坛
/*
31.911010, 118.892272
31.911080, 118.892530
31.910974, 118.892572
31.910906, 118.892324
*/

pm[0].Longitude =  118.892272;
pm[0].Latitude = 31.911010;

pm[1].Longitude = 118.892530;
pm[1].Latitude = 31.911080;

pm[2].Longitude = 118.892572;
pm[2].Latitude = 31.910974;
	
pm[3].Longitude = 118.892324;
pm[3].Latitude = 31.910906;



planMarkingCount = 4 ;

double lengthMax = INFINITY;

double lengthCur = 0;

for(uint8_t i = 0 ; i < planMarkingCount ; i++){
	
	nextPlanMarking.Longitude = pm[i].Longitude;
	
	nextPlanMarking.Latitude = pm[i].Latitude;
	
	lengthCur = calculateDistance();
	
	if( lengthCur < lengthMax ){
	
	curPlanMarkingFlag = i ;
	
	lengthMax = lengthCur ;
		
	
		
	}
	
	quickSendNum("init plan ",i);
	
	quickSendDouble("init length ",lengthCur);
	
	HAL_IWDG_Refresh(&hiwdg);
	
}

	nextPlanMarking.Longitude = pm[curPlanMarkingFlag].Longitude;
	
	nextPlanMarking.Latitude = pm[curPlanMarkingFlag].Latitude;
	
	quickSendNum("sure plan ",curPlanMarkingFlag);

	quickSendDouble("sure length ",calculateDistance());

}

//根据当前的gps数据 更新下一个路线规划 存放于  nextPlanMarking
void updatePlanMarking(void){
	
	//如果距离目标经纬度距离小于设定 deviationDistance 则修改下一个前进的目标点
	if(calculateDistance() <= deviationDistance){
		//如果达到最后一个标记点 此时取余操作归0 
		curPlanMarkingFlag = ( curPlanMarkingFlag + 1 ) % planMarkingCount;
		//重置目标点 复原到起点
		nextPlanMarking.Latitude = pm[curPlanMarkingFlag].Latitude;
		nextPlanMarking.Longitude = pm[curPlanMarkingFlag].Longitude;
	}
	
}

//通过当前经纬度 和 目标经纬度    
/*计算出当前位置 和目标位置的的朝向*/
double calculateBearing() {
	
	//当前的经纬度数据
	double lat1 = curGPSData.latitude; double lon1 = curGPSData.longitude ;
	//目标经纬度数据
	double lat2 = nextPlanMarking.Latitude; double lon2 = nextPlanMarking.Longitude;
	
    lat1 = degrees_to_radians(lat1);
    lon1 = degrees_to_radians(lon1);
    lat2 = degrees_to_radians(lat2);
    lon2 = degrees_to_radians(lon2);

    double dlon = lon2 - lon1;

    double x = sin(dlon) * cos(lat2);
    double y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon);

    double initial_bearing = atan2(x, y);
    // 转换到0-360范围
    initial_bearing = fmod((initial_bearing * 180.0 / M_PI + 360.0), 360.0);

    return initial_bearing;
}

//通过当前经纬度 和 目标经纬度    
/*计算出当前位置 和目标位置的距离*/
double calculateDistance() {
	
	//当前的经纬度数据
	double lat1 = curGPSData.latitude; double lon1 = curGPSData.longitude ;
	//目标经纬度数据
	double lat2 = nextPlanMarking.Latitude; double lon2 = nextPlanMarking.Longitude;
	
    lat1 = degrees_to_radians(lat1);
    lon1 = degrees_to_radians(lon1);
    lat2 = degrees_to_radians(lat2);
    lon2 = degrees_to_radians(lon2);

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

// 将角度转换为弧度 这个是辅助计算 无需调用
double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}
