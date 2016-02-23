// Simple demo of three threads
// LED blink thread, print thread, and idle loop
#include <FreeRTOS_AVR.h>

const uint8_t LED_PIN = 13;

volatile uint32_t count = 0;

// handle for blink task
xTaskHandle blink;

//------------------------------------------------------------------------------
// high priority for blinking LED
static void vLEDFlashTask(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  
  // Flash led every 200 ms.
  for (;;) {
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);
    
    // Sleep for 50 milliseconds.
    vTaskDelay((50L * configTICK_RATE_HZ) / 1000L);
    
    // Turn LED off.
    digitalWrite(LED_PIN, LOW);
    
    // Sleep for 150 milliseconds.
    vTaskDelay((150L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
static void vPrintTask(void *pvParameters) {
  while (1) {
    // Sleep for one second.
    vTaskDelay(configTICK_RATE_HZ);
    
//    // Print count for previous second.
//    Serial.print(F("Count: "));
//    Serial.print(count);
//    
//    // Print unused stack for threads.
//    Serial.print(F(", Unused Stack: "));
//    Serial.print(uxTaskGetStackHighWaterMark(blink));
//    Serial.print(' ');
//    Serial.print(uxTaskGetStackHighWaterMark(0));
//    Serial.print(' ');
//    Serial.println(freeHeap());
    
    Serial.print(analogRead(0));  Serial.print(",");    //Iload
    Serial.print(analogRead(1));  Serial.print(",");    //Ibatt
    Serial.print(analogRead(3));  Serial.print(",");    //Ihub16
    Serial.print(analogRead(4));  Serial.print(",");    //Ihub8
    Serial.print(analogRead(5));  Serial.print(",");    //Ichrg
    Serial.print(analogRead(6));  Serial.print(",");    //Iarr
    Serial.print(analogRead(8));  Serial.print(",");    //Varr
    Serial.print(analogRead(9));  Serial.print(",");    //Vload
    Serial.print(analogRead(10));  Serial.print(",");    //Vbatt
    Serial.print(analogRead(11));  Serial.print(",");    //Vhub1
    Serial.print(analogRead(12));  Serial.println(",");    //Vhub2

    
    // Zero count.
    count = 0;
  }
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}

  // create blink task
  xTaskCreate(vLEDFlashTask,
    (signed portCHAR *)"Task1",
    configMINIMAL_STACK_SIZE + 50,
    NULL,
    tskIDLE_PRIORITY + 2,
    &blink);

  // create print task
  xTaskCreate(vPrintTask,
    (signed portCHAR *)"Task2",
    configMINIMAL_STACK_SIZE + 100,
    NULL,
    tskIDLE_PRIORITY + 1,
    NULL);

  // start FreeRTOS
  vTaskStartScheduler();

  // should never return
  Serial.println(F("Die"));
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  while(1) {
    // must insure increment is atomic
    // in case of context switch for print
    noInterrupts();
    count++;
    interrupts();
  }
}
