#include <TASK/taskA.h>
#include <GLOBALS/globals.h>

extern SemaphoreHandle_t mutex_1;
extern SemaphoreHandle_t mutex_2;

//*****************************************************************************
// Tasks

// Task B (bassa priorità)
// il comando xSemaphoreTake() ha una limitazione sul tempo massimo di attesa
// durante l'acquisizione del mutex
void doTaskB(void *parameters)
{

  // Ciclo infinito
  while (1)
  {

    if (mutex_1 == NULL || mutex_2 == NULL)
    {
      Serial.println("Task B ERRORE: mutex nulli");
      while (1)
      {
        ;
      }
    }

    // Nota: l'ordine di acquisizione dei due mutex è lo stesso del Task A
    // Prende il mutex 2 (la pausa di 3ms induce il deadlock con l'altro task)
    if (xSemaphoreTake(mutex_1, pdMS_TO_TICKS(mutex_timeout)) == pdTRUE)
    {
      Serial.println("Il Task B prende il mutex 1");
      vTaskDelay(pdMS_TO_TICKS(3));

      // Prende il mutex 2
      if (xSemaphoreTake(mutex_2, pdMS_TO_TICKS(mutex_timeout)) == pdTRUE)
      {
        Serial.println("Il Task B prende il mutex 2");

        // Sezione critica protetta con 2 mutexes
        Serial.println("Il Task B sta elaborando dati...");
        vTaskDelay(pdMS_TO_TICKS(500));

        // Rilascia il mutexes 1
        Serial.println("Il Task B rilascia il mutex 2 e il mutex 1");
        xSemaphoreGive(mutex_2);
        xSemaphoreGive(mutex_1);
      }
      else
      {
        Serial.println("Il Task B non ha preso il mutex 2 (timeout)");

        // Rilascia il mutexes 1
        Serial.println("Il Task B rilascia il mutex 1");
        xSemaphoreGive(mutex_1);
      }
    }
    else
    {
      Serial.println("Il Task B non ha preso il mutex 1 (timeout)");
    }

    // Pausa per consentire l'esecuzione dell'altro task
    Serial.println("Il Task B va in pausa");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}