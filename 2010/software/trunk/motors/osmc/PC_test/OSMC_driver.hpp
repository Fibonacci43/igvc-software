#include "ArduinoInterface.h"
#include "ArduinoCmds.hpp"

#include "quadCoderDriver.hpp"

//if this is defined, nothing is sent to the motors!, it is just printed to stdout
#define MOTOR_SIMULATE 1
#define ENCODER_SIMULATE 1
//#define CURRENT_SIMULATE 1

//following gnu libc tradition,
//a return value of false is success (eg, code 0)
//a return value of true is failure (eg, code nonzero)

class OSMC_driver
{

	public:
	OSMC_driver();
	OSMC_driver(byte motor_iface, byte encoder_iface);
	~OSMC_driver();

	#if 0
	bool connect();
	bool is_connected();
	#endif

	current_reply_t getCurrentData();
	joystick_reply_t getJoystickData();

	//make 0 vel a nonrolling stop
	bool setMotorPWM(byte rightDir, byte rightDutyCycle, byte leftDir, byte leftDutyCycle);

	//shims from old motor
	bool set_motors(int leftPWM, int rightPWM);
	bool set_motors(int pwm);
	int set_heading(int iFwdVelocity, int iRotation);

	void getNewVel_dumb(const double rtarget, const double ltarget, const double rvel, const double lvel, const int rmset, const int lmset, int& out_rmset, int& out_lmset);

	//follow path
	void followCirc(double radius, double vmag);

	//add these commands
	//bool enableMotor();
	//bool disableMotor();

	//interact with the associated encoder
	bool getEncoderVel(double& rvel, double& lvel);
	bool getEncoderData(new_encoder_pk_t& pk);

	//run one pd loop
	void setVel_pd(double left, double right);
	bool updateVel_pd();

	//get state
	void getVelSet_pd();
	void getLastPWMSent(byte& r, byte& l);

	//set vel from vision vector
	bool set_vel_vec(const double y, const double x);

	private:
	ArduinoInterface ai;

	quadCoderDriver* encoder;

	const static int _max_speed_;
	const static int MINREQSPEED;

	bool m_connected;

	//the pwm settings
	byte ldir, rdir;
	byte lpwm, rpwm;

	//left and right vels for pd loop
	double lvgoal;// m/s desired
	double rvgoal;// m/s desired
	double last_l_error;// m/s error at time t
	double last_r_error;// m/s error at time t
	double t;// seconds since unix epoch
	void getNewVel_pd(const double now_lvel, const double now_rvel, const double dt, int& out_r, int& out_l);
};
