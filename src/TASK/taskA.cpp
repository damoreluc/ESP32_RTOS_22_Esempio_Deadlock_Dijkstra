#include <TASK/taskA.h>
#include <GLOBALS/globals.h>

extern SemaphoreHandle_t mutex_1;
extern SemaphoreHandle_t mutex_2;

//*****************************************************************************
// Tasks

// Task A (alta priorità)
// il comando xSemaphoreTake() ha una limitazione sul tempo massimo di attesa
// durante l'acquisizione del mutex
void doTaskA(void *parameters)
{

  // Ciclo infinito
  while (1)
  {

    if (mutex_1 == NULL || mutex_2 == NULL)
    {
      Serial.println("Task A ERRORE: mutex nulli");
      while (1)
      {
        ;
      }
    }

    // Prende il mutex 1 (la pausa di 3ms induce il deadlock con l'altro task)
    if (xSemaphoreTake(mutex_1, pdMS_TO_TICKS(mutex_timeout)) == pdTRUE)
    {
      Serial.println("Il Task A prende il mutex 1");
      vTaskDelay(pdMS_TO_TICKS(3));

      // Prende il mutex 2
      if (xSemaphoreTake(mutex_2, pdMS_TO_TICKS(mutex_timeout)) == pdTRUE)
      {
        Serial.println("Il Task A prende il mutex 2");

        // Sezione critica protetta con 2 mutexes
        Serial.println("Il Task A sta elaborando dati...");
        vTaskDelay(pdMS_TO_TICKS(500));

        // Rilascia il mutexes 2
        Serial.println("Il Task A rilascia il mutex 2 e il mutex 1");
        xSemaphoreGive(mutex_2);
        xSemaphoreGive(mutex_1);
      }
      else
      {
        Serial.println("Il Task A non ha preso il mutex 2 (timeout)");

        // Rilascia il mutexes 1
        Serial.println("Il Task A rilascia il mutex 1");
        xSemaphoreGive(mutex_1);
      }
    }
    else
    {
      Serial.println("Il Task A non ha preso il mutex 1 (timeout)");
    }

    // Pausa per consentire l'esecuzione dell'altro task
    Serial.println("Il Task A va in pausa");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}