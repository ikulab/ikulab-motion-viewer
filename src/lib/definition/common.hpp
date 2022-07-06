#pragma once

const int MAX_FRAMES_IN_FLIGHT = 2;

const int NUM_OF_JOINT_ID = 256;
const int NUM_OF_ID_OTHER_THAN_JOINTS = 2;
const int NUM_OF_ID = NUM_OF_JOINT_ID + NUM_OF_ID_OTHER_THAN_JOINTS;

const int FLOOR_ID = NUM_OF_JOINT_ID + 0;
const int DEBUG_OBJECT_ID = NUM_OF_JOINT_ID + 1;