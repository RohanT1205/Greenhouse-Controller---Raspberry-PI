/** @brief Serial: 913a7526
    file: ghc.c
 *
 */
#include "ghcontrol.h"
/** Monitors the greenhouse environment based on sensor readings and predefined targets
 * @version CENG153, serial: 913a7526
 * @author Paul Moggach
 * @author Rohan Trikha
 * @since 2024-02-18
 * @param void
 * @return int
*/
int main(void)
{
	time_t now;
	reading_s creadings = {0};
	control_s ctrl = {0};
	setpoint_s sets = {0};
	setpoint_s spts = {0};
	sets = GhSetTargets();
	alarmlimits_s alimits = {0};
	alarm_s warn[NALARMS];
	int logged;
	alarm_s *arecord;
	arecord = (alarm_s *)calloc(1,sizeof(alarm_s));
	if (arecord == NULL)
	{
        printf("\nCannot allocate memory\n");
        return EXIT_FAILURE;
	}

	GhControllerInit();
	struct fb_t *fb;
    fb=ShInit(fb);

	while (true)
	{
		now = time(NULL);
		creadings = GhGetReadings();
		logged = GhLogData("ghdata.txt",creadings);
		GhDisplayAll(creadings,sets,fb);
		GhDisplayReadings(creadings);
		ctrl = GhSetControls(sets,creadings);
		alimits = GhSetAlarmLimits();
		arecord = GhSetAlarms(arecord,alimits,creadings);
		//GhDisplayTargets(spts);
		//GhDisplayControls(ctrl);
		//GhDisplayAlarms(arecord);
		GhDelay(GHUPDATE);
	}

	return EXIT_FAILURE;
}
