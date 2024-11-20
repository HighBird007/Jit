#include "PlanPath.h"

PathMarking pm[10];

PathMarking nextPlanMarking;
// 将角度转换为弧度 这个是辅助计算 无需调用
double degrees_to_radians(double degrees);

//需要前往下一个标记的经纬度
uint32_t curPlanMarkingFlag = 0;

//存放标记点数量
uint32_t planMarkingCount = 4 ;
//预设好的经纬度路线规划 预设好之后 需要修改上面的存放数
void planMarkingInit(void){
	
	pm[0].Longitude=118.960981926;
	pm[0].Latitude=31.807021671;
	pm[1].Longitude=118.961000041;
	pm[1].Latitude=31.807395369;
	pm[2].Longitude=118.961321906;
	pm[2].Latitude=31.807391346;
	pm[3].Longitude=118.961328611;
	pm[3].Latitude=31.807120443;
	
}

//根据当前的gps数据 更新下一个路线规划
void updatePlanMarking(void){
	//更新当前的gps位置
	updateCurrentGPSData();
	
	//如果距离目标经纬度距离小于设定 deviationDistance 则修改下一个前进的目标点
	if(calculateDistance() <= deviationDistance){
		//如果达到最后一个标记点 此时取余操作归0 
		curPlanMarkingFlag = ( curPlanMarkingFlag + 1 ) % planMarkingCount;
		//重置目标点 复原到起点
		nextPlanMarking.Latitude = pm[curPlanMarkingFlag].Latitude;
		nextPlanMarking.Longitude = pm[curPlanMarkingFlag].Longitude;
	}
	
	/* 下面运行的需要删除 放入其他方法中 现在只是测试*/
	//计算出 现在应该的朝向
     nextHeadingAngle = calculateBearing();
	
	float t = hmcPIDController_Update(&hmcPid,nextHeadingAngle,hmcGetHeading());
	
	quickSendNum("hmc pid",t);
	//转向舵机转弯
	turnHeading(t);
	
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
