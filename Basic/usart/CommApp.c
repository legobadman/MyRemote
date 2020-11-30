#include "CommApp.h"

#define MAX_LEN 32
volatile uint8_t UdataBuf[MAX_LEN];

static uint8_t checksum = 0;
uint8_t bufP = 0;//static
static uint8_t validDataLen = 0;

void 