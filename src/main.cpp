#include "main.h"
using namespace okapi;

Motor frontLeft = 12_mtr;
Motor backLeft = 11_mtr;
Motor frontRight = 19_rmtr;
Motor backRight = 20_rmtr;
Motor intakeLeft = 2_mtr;1
Motor intakeRight = 9_rmtr;
Motor liftLeft = 1_mtr;
Motor liftRight = 10_rmtr;

auto liftControlL = AsyncControllerFactory::posIntegrated(liftLeft);
auto liftControlR = AsyncControllerFactory::posIntegrated(liftRight);

auto hotPocket = ChassisControllerFactory::create({frontLeft, backLeft},
	{frontRight, backRight}, AbstractMotor::gearset::green, {4.125_in, 15.5_in});

ControllerButton btnUp(ControllerDigital::L1);
ControllerButton btnDown(ControllerDigital::L2);
ControllerButton btnIn(ControllerDigital::R1);
ControllerButton btnOut(ControllerDigital::R2);

const double liftHeight = 15.5;
const double liftLength = 18;
const double angles[3] = {asin((1-liftHeight)/liftLength),
	asin((20.2-liftHeight)/liftLength), asin((26.2-liftHeight)/liftLength)};

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {}

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
	liftControlL.setTarget(angles[0]);
	liftControlR.setTarget(angles[0]);
	hotPocket.moveDistance(36_in);
	intakeLeft.moveVoltage(-12000);
	intakeRight.moveVoltage(-12000);
	pros::delay(1000);
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
	int goal = 0;

	master.setText(1, 1, "1");

	while (true) {
		hotPocket.tank(master.getAnalog(ControllerAnalog::leftY)*0.75,
			master.getAnalog(ControllerAnalog::rightY)*0.75);

		if (btnIn.isPressed()) {
			intakeLeft.moveVoltage(12000);
			intakeRight.moveVoltage(12000);
		} else if (btnOut.isPressed()) {
			intakeLeft.moveVoltage(-12000);
			intakeRight.moveVoltage(-12000);
		} else {
			intakeLeft.moveVoltage(0);
			intakeRight.moveVoltage(0);
		}

		if (btnUp.isPressed()) {
			liftLeft.moveVoltage(12000);
			liftRight.moveVoltage(12000);
		} else if (btnDown.isPressed()) {
			liftLeft.moveVoltage(-2000);
			liftRight.moveVoltage(-2000);
		} else {
			liftLeft.moveVoltage(0);
			liftRight.moveVoltage(0);
		}

	  /*if (btnUp.changedToPressed() && goal < 2) {
			goal++;
			liftControlL.setTarget(angles[goal]);
			liftControlR.setTarget(angles[goal]);
		} else if (btnDown.changedToPressed() && goal > 0) {
			goal--;
			liftControlL.setTarget(angles[goal]);
			liftControlR.setTarget(angles[goal]);
		}*/

		if (goal == 0) {
			master.setText(1, 1, "0");
		} else if (goal == 1) {
			master.setText(1, 1, "1");
		} else if (goal == 2) {
			master.setText(1, 1, "2");
		}

		pros::delay(10);
	}
}
