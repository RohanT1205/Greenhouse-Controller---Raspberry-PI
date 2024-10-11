/** @brief Gh control functions
*   @file ghcontrol.c
*/
#include "ghcontrol.h"

// Alarm Message Array

const char alarmnames[NALARMS][ALARMNMSZ] = {"No Alarms","High Temperature","Low Temperature","High Humidity",
"Low Humidity","High Pressure","Low Pressure"};

/** Prints Gh Controller Title
 * @version CENG153, serial 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param sname string with Operator's name
 * @return void
*/
void GhDisplayHeader(const char * sname)
{
	fprintf(stdout,"%s's CENG153 Greenhouse Controller\n", sname);
}
/** Takes integer from range and returns random value in range
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param range
 * @return int
*/
int GhGetRandom(int range)
{
	return rand() % range;
}
/** Delays display for a specific time
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param milliseconds with integer type
 * @return void
*/
void GhDelay(int milliseconds)
{
	long wait;
	clock_t now,start;

	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now-start) < wait )
	{
		now = clock();
	}
}
/** Prints a header and initializes random number generator
 * @version CENG153, serial:913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return void
*/
void GhControllerInit(void)
{
	//GhDisplayHeader("Rohan Trikha");
	initscr();
	srand((unsigned) time(NULL));
}
/** Prints heater and humidifier control values
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param heater & humidifier pointers under integer type
 * @return void
*/
void GhDisplayControls(control_s ctrl)
{
    fprintf(stdout, "Controls\tHeater:%d\tHumidifier:%d\n",ctrl.heater,ctrl.humidifier);
}
/** Prints random temp, humid, and press values within a range
 * @version CENG153, serial:9137526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param timestamp rtime and sensor readings dreads
 * @return void
*/
void GhDisplayReadings(reading_s rdata)
{
	//fprintf(stdout,"\nUnit: %LX %sReadings\tT: %5.1fC\tH: %5.1f%%\tP: %6.1fmb\n", ShGetSerial(),ctime(&rdata.rtime),rdata.temperature,rdata.humidity,rdata.pressure);
    mvprintw(1,1,"\nUnit: %LX %sReadings\tT: %5.1fC\tH: %5.1f%%\tP: %6.1fmb\n", ShGetSerial(),ctime(&rdata.rtime),rdata.temperature,rdata.humidity,rdata.pressure);
    refresh();
}
/** Prints on or off for heater and humidifier
 * @version CENG153, serial:913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return void
*/
void GhDisplayTargets(setpoint_s spts)
{
	fprintf(stdout, "Setpoints\tT: %5.1lfC\tH: %5.1lf%%\n",STEMP,SHUMID );
}
/** Prints heater & humidifier as on or off in relation to constant val
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param heater and humidifier pointers of int type with sensor readings
 * @return void
*/
void GhsetControls(int *heater, int *humidifier, reading_s rdata)
{
    if (rdata.temperature<STEMP){ *heater = ON;}
    else {*heater = OFF;}

    if (rdata.humidity<SHUMID){*humidifier = ON;}
    else {*humidifier = OFF;}
}
/** Prints random humidity value or prints fixed humidity value depending on SIMULATE
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return float
*/
float GhGetHumidity(void)
{
#if SIMHUMIDITY
	return GhGetRandom(USHUMID - LSHUMID) + LSHUMID;
#else
	return ShGetHumidity();
#endif
}
/** Prints random pressure value or prints fixed pressure value depending on SIMULATE
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return float
*/
float GhGetPressure(void)
{
#if SIMPRESSURE
	return GhGetRandom(USPRESS - LSPRESS) + LSPRESS;
#else
	return ShGetPressure();
#endif
}
/** Prints random temperature value or prints fixed temperature value depending on SIMULATE
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return float
*/
float GhGetTemperature(void)
{
#if SIMTEMPERATURE
	return GhGetRandom(USTEMP - LSTEMP) + LSTEMP;
#else
	return ShGetTemperature();
#endif
}
/** Prints temp,humidity,and pressure and the current time
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return struct
*/
reading_s GhGetReadings(void)
{
    reading_s now = {0};

    now.rtime = time(NULL);
    now.temperature = GhGetTemperature();
    now.humidity = GhGetHumidity();
    now.pressure = GhGetPressure();
    return now;
}
/** Creates and returns a structure representing target temp and humidity for the greenhouse
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return struct
*/
setpoint_s GhSetTargets(void)
{
    setpoint_s cpoints = {0};
    setpoint_s spts = {0};
    spts = GhRetrieveSetpoints("setpoints.dat");
    if (cpoints.temperature == 0)
    {
        cpoints.temperature = STEMP;
        cpoints.humidity = SHUMID;
        GhSaveSetpoints("setpoints.dat",cpoints);
    }
    return cpoints;
}
/** Determines the heater and humidifier status based on sensor readings
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param struct
 * @return struct
*/
control_s GhSetControls(setpoint_s target,reading_s rdata)
{
    control_s cset = {0};

    if (rdata.temperature<STEMP){cset.heater = ON;}
    else {cset.heater = OFF;}

    if (rdata.humidity<SHUMID){cset.humidifier = ON;}
    else {cset.humidifier = OFF;}

    return cset;
}
/** Logs readings data to a file ghdata.txt
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-07
 * @param struct, char
 * @return int
*/
int GhLogData(char * fname, reading_s ghdata)
{
    FILE* fp;
    char ltime [CTIMESTRSZ];

    fp = fopen(fname,"a");
    if (fname == NULL)
    {
        return 0;
    }

    strcpy(ltime,ctime(&(ghdata.rtime)));

    ltime[3,7,10,19] = ',';


    fprintf(fp, "\n%.24s,%5.1lf,%5.1lf,%6.1lf",ltime,ghdata.temperature,ghdata.humidity,ghdata.pressure);
    fclose(fp);
    return 1;
}
/** Saving setpoints to ghdata.txt
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-07
 * @param struct, char
 * @return int
*/
int GhSaveSetpoints(char * fname, setpoint_s spts)
{
    FILE* fp;
    fp = fopen(fname, "w");
    if (fp == NULL)
    {
        return 0;
    }

    if (fp != NULL)
    {
        fwrite(&spts,sizeof(setpoint_s), 1, fp);
        fclose(fp);
    }

    return 1;
}
/** Retrieve setpoints from ghdata.txt and return as struct
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-07
 * @param char
 * @return struct
*/
setpoint_s GhRetrieveSetpoints(char * fname)
{
    setpoint_s spts = {0.0};
    FILE* fp;
    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        return spts;
    }

    if (fp != NULL)
    {
        fread(&spts,sizeof(setpoint_s), 1, fp);
        fclose(fp);
    }

    return spts;

}
/** Sets green vertical bar to display readings
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-15
 * @param int,unint8_t,struct
 * @return int
*/
int GhSetVerticalBar(int bar,COLOR_SENSEHAT pxc, uint8_t value, struct fb_t *fb)
{
    int i = 0;

    value < 7;
    if (value > 7)
    {
        value = 7;
    }
    if (bar < 0 || bar >= 8 || value < 0 || value >= 8)
    {
        return EXIT_FAILURE;
    }
    for (i=0; i<=value; i++)
    {
        ShLightPixel(i, bar, pxc, fb);
    }
    for (i=value+1;i<8;i++)
    {
        ShLightPixel(i, bar, BLACK, fb);
    }

    return EXIT_SUCCESS;
}
/** Displays the setpoints and varying temp, humid, press
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-15
 * @param struct
 * @return void
*/
void GhDisplayAll(reading_s rd, setpoint_s sd, struct fb_t *fb)
{
    int rv,sv,avh,avl;
    COLOR_SENSEHAT pxc;
    ShWipeScreen(BLACK, fb);

    rv = (8.0 *(((rd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
    GhSetVerticalBar(TBAR,GREEN,rv,fb);
    sv = (8.0 *(((sd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
    ShLightPixel(sv,TBAR,MAGENTA,fb);


    rv = (8.0 *(((rd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
    GhSetVerticalBar(HBAR,GREEN,rv,fb);
    sv = (8.0 *(((sd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
    ShLightPixel(sv,HBAR,MAGENTA,fb);

    rv = (8.0 *(((rd.pressure-LSPRESS)/(USPRESS-LSPRESS))+0.05))-1.0;
    GhSetVerticalBar(PBAR,GREEN,rv,fb);

}
/** Sets predefined upper and lower limits for temp, humid, and press
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-28
 * @param void
 * @return alarmlimits_s
*/
alarmlimits_s GhSetAlarmLimits(void)
{
    alarmlimits_s calarm;

    calarm.hight = UPPERATEMP;
    calarm.lowt = LOWERATEMP;
    calarm.highh = UPPERAHUMID;
    calarm.lowh = LOWERAHUMID;
    calarm.highp = UPPERAPRESS;
    calarm.lowp = LOWERAPRESS;
    return calarm;
}
/** Checks sensor readings and sets corresponding alarm statuses
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-28
 * @param alarm_s, alarmlimits_s, readin_s
 * @return void
*/
alarm_s * GhSetAlarms (alarm_s * head, alarmlimits_s alarmpt, reading_s rdata) {

    if (rdata.temperature >= alarmpt.hight) {

         GhSetOneAlarm(HTEMP,rdata.rtime,rdata.temperature,head);
    } else {
        head = GhClearOneAlarm(HTEMP,head);
    }

    if (rdata.temperature <= alarmpt.lowt) {

         GhSetOneAlarm(LTEMP,rdata.rtime,rdata.temperature,head);
    } else {
        head = GhClearOneAlarm(LTEMP,head);
    }

    if (rdata.humidity >= alarmpt.highh) {

         GhSetOneAlarm(HHUMID,rdata.rtime,rdata.humidity,head);
    } else {
        head = GhClearOneAlarm(HHUMID,head);
    }

    if (rdata.humidity <= alarmpt.lowh) {

         GhSetOneAlarm(LHUMID,rdata.rtime,rdata.humidity,head);
    } else {
        head = GhClearOneAlarm(LHUMID,head);
    }

    if (rdata.pressure >= alarmpt.highp) {

         GhSetOneAlarm(HPRESS,rdata.rtime,rdata.pressure,head);
    } else {
        head = GhClearOneAlarm(HPRESS,head);
    }

    if (rdata.pressure <= alarmpt.lowp) {

         GhSetOneAlarm(LPRESS,rdata.rtime,rdata.pressure,head);
    } else {
        head = GhClearOneAlarm(LPRESS,head);
    }

    return head;
}

/** Displays current alarm statuses along with their timestamps
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-03-28
 * @param alarm_s
 * @return void
*/
void GhDisplayAlarms(alarm_s * head)
{
    alarm_s *cur = head;
    int Alarm = 0;
    printf("\nAlarms\n");
    while (cur != NULL)
    {
        if (cur->code != NOALARM)
        {
            printf("%s %s", alarmnames[cur->code], ctime(&cur->atime));
            Alarm = 1;
        }
        cur = cur->next;
    }

    if (Alarm == 0)
    {
        printf("No Alarm\n");
    }
}

/** Adds alarms to linked list, avoiding duplicates and updates list
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-04-13
 * @param alarm_e, time_t, double, alarm_s
 * @return int
*/
int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarm_s * head)
{
    alarm_s *cur = head;
    alarm_s *last = NULL;

    while (cur != NULL)
    {
        if (cur->code == code)
        {
            return 0;
        }
        last = cur;
        cur = cur->next;
    }

    cur = (alarm_s *)calloc(1, sizeof(alarm_s));

    cur->code = code;
    cur->atime = atime;
    cur->value = value;
    cur->next = NULL;

    if (last != NULL)
    {
        last->next = cur;
    }
    cur->code = code;
    cur->atime = atime;
    cur->value = value;
    cur->next = NULL;

    return 1;
}
/** Removes alarm from linked list if it is duplicate
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-04-13
 * @param alarm_e, alarm_s
 * @return alarm_s
*/
alarm_s * GhClearOneAlarm(alarm_e code, alarm_s * head)
{
    alarm_s *cur = head;
    alarm_s *last = NULL;

    while (cur != NULL)
    {
        if (cur->code == code)
        {
            if (last == NULL)
            {
                head = cur->next;
            }
            else
            {
                last->next = cur->next;
            }
            free(cur);
            return head;
        }
        last = cur;
        cur = cur->next;
    }

    return head;
}
