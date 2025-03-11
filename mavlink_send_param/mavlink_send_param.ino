#include <MAVLink.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  sendParamSetMessage();
}

void loop() {
  sendParamSetMessage();
  delay(200);
  sendParamValue("TEST_PARAM", 3.14f, MAVLINK_TYPE_FLOAT);
  delay(200);
}

void sendParamSetMessage() {
  uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
  mavlink_message_t msg;
  mavlink_param_set_t param_set;
  const char* param_id = "RESET";
  float param_value = 1.0;
  uint8_t param_type = MAV_PARAM_TYPE_REAL32;

  mavlink_msg_param_set_pack(1, 1, &msg, 1, 1, param_id, param_value, param_type);

  uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
  Serial.write(buffer, len);
  Serial3.write(buffer, len);
}

void sendParamValue(const char* paramId, float paramValue, uint8_t paramType) {
  mavlink_message_t msg;
  mavlink_param_value_t param;

  strncpy(param.param_id, paramId, sizeof(param.param_id)); 
  param.param_value = paramValue; 
  param.param_type = paramType; 
  param.param_count = 1; 
  param.param_index = 0; 

  mavlink_msg_param_value_pack(1, 1, &msg, param.param_id, param.param_value, param.param_type, param.param_count, param.param_index);

  uint8_t mavBuffer[255]; 
  uint16_t len = mavlink_msg_to_send_buffer(mavBuffer, &msg);
  Serial.write(mavBuffer, len); 
}