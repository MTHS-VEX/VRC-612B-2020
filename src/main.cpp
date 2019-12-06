#include "main.h"

Motor frontLeft = 12_mtr;
Motor backLeft = 11_mtr;
Motor frontRight = 19_rmtr;
Motor backRight = 20_rmtr;
Motor intakeLeft = 2_mtr;
Motor intakeRight = 9_rmtr;
Motor liftLeft = 1_mtr;
Motor liftRight = 10_rmtr;

auto liftControl = AsyncControllerFactory::posIntegrated(liftLeft,liftRight);
auto hotPocket = ChassisControllerFactory::create({frontLeft, backLeft},
	{frontRight, backRight}, AbstractMotor::gearset::green, {4.125_in, 16.5_in});

ControllerButton btnUp(ControllerDigital::A);
ControllerButton btnDown(ControllerDigital::B);
ControllerButton btnIn(ControllerDigital::up);
ControllerButton btnOut(ControllerDigital::down);

const double liftHeight = 17.5_in-2_in;
const double liftLength = 18_in;
const double heights[3] = {2_in, 20.2_in, 26.2_in};

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {

}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	liftControl.setTarget(arcsin((1_in-liftHeight)/liftLength));
	hotPocket.moveDistance(6_in);
	hotPocket.turnAngle(90_deg);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Controller master;
	int goalHeight = 0;

	while (true) {
		drive.tank(masterController.getAnalog(ControllerAnalog::leftY),
			masterController.getAnalog(ControllerAnalog::rightY));

		if (btnIn.isPressed()) {
			intakeLeft.moveVoltage(12000);
			intakeRight.moveVoltage(12000);
		} else if (btnOut.isPressed()) {
			intakeLeft.moveVoltage(-12000);
			intakeRight.moveVoltage(-12000);
		}

		if (btnUp.changedToPressed() && goalHeight < 2) {
			goalHeight++;
			liftControl.setTarget(arcsin((heights[goalHeight]-liftHeight)/liftLength));
		} else if (btnDown.changedToPressed() && goalHeight > 0) {
			goalHeight--;
			liftControl.setTarget(arcsin((heights[goalHeight]-liftHeight)/liftLength));
		}

		pros::delay(10);
	}
}
