#include "main.h"
#include <stdio.h>
#include <string.h>

#include "morseBinaryTree.h"
#include "morseTranslater.h"
#include "keyerSettings.h"
#include "pinsLocations.h"
#include "speekerKeyPlayer.h"
#include "i2clcd.h"
#include <random>

UART_HandleTypeDef huart2;
RNG_HandleTypeDef RngHandle;
I2C_HandleTypeDef hi2c1;

#define MAX_SPELLER_WORD (15) 
#define MAX_SPELLER_WORD_AMOUNT (13) 
const char WORDLIST[MAX_SPELLER_WORD_AMOUNT][MAX_SPELLER_WORD] = {
  {"KEYER\0"},
  {"TEST\0"},
  {"TYLER\0"},
  {"BEANS\0"},
  {"HELLO\0"},
  {"EMILY\0"},
  {"WORLD\0"},
  {"THINGS\0"},
  {"33WSE3\0"},
  {"KYLKYL\0"},
  {"CONOR\0"},
  {"JOHN\0"},
  {"CLOCK\0"}
};

enum WhiteSpaceState
{
  RecievedLetter = 0,
  ProcessedLetter = 1,
  StartOfWord = 2,
};

enum AllowedInputContact
{
  Unrestricted = 0,
  AwaitingRing = 1,
  AwaitingTip = 2,
};

extern "C" int _write(int file, char *ptr, int len)
{
  if (HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY) == HAL_OK)
  {
    return len;
  }
  return -1;
}

void getRandomNumber(uint32_t* last)
{
  uint32_t randomNumber;
  while ((HAL_RNG_GenerateRandomNumber(&RngHandle, &randomNumber) != HAL_OK) || ((randomNumber % MAX_SPELLER_WORD_AMOUNT) == ((*last) % MAX_SPELLER_WORD_AMOUNT)));
  (*last) = randomNumber;
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);

extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2); // Call HAL's UART ISR handler
}

void keyerRoutine();
bool spellerRoutine(const char[MAX_SPELLER_WORD], bool);
bool getKeyerWord(char* currentWord = NULL, const char* wantedWord = NULL, size_t size = 0);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  __HAL_RCC_RNG_CLK_ENABLE();
  __I2C1_CLK_ENABLE();
  setvbuf(stdout, NULL, _IONBF, 0);
  SystemClock_Config(); 
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();

  lcd_init(); 
  //lcd_write("test", 0, 0);
  // malloc(sizeof(char*) * 10);

  HAL_Delay(1000);
  
  RngHandle.Instance = RNG;

  if (HAL_RNG_Init(&RngHandle) != HAL_OK)
  {
      Error_Handler();
  }
  HAL_Delay(1000); 

  // lcd address 0x27

  printf("\n");
  SpeekerPlayer.playTest(true);
  printf(" Program\r\n"); 
  HAL_Delay(200); 
  SpeekerPlayer.playChar('R');
  printf("Settings:\r\n WPM: %d\r\n Dit: %dms\r\n Dah: %dms\r\n", WPM, shortSignalLengthMS, longSignalLengthMS);
  for (int i = 0; i < 3; i++)
  {
    HAL_Delay(500);
    printf("."); 
  }
  HAL_Delay(1000);
  printf("\n");

  uint32_t randomNumber = 0;

  while(true)
  {
    getRandomNumber(&randomNumber);
    spellerRoutine(WORDLIST[randomNumber % MAX_SPELLER_WORD_AMOUNT], true);
  }
}

void keyerRoutine()
{
  while (getKeyerWord());
}

bool spellerRoutine(const char wantedWord[MAX_SPELLER_WORD], bool showAnswer)
{
  printf("\nSpell: %s\n", wantedWord);
  if (showAnswer)
  {
    SpeekerPlayer.playStr(wantedWord, MAX_SPELLER_WORD, true, true);
    printf("\n");
  }
  printf("Start Output: ");
  char currentWord[MAX_SPELLER_WORD] = {0};
  while(!getKeyerWord(currentWord, wantedWord, MAX_SPELLER_WORD));
  printf("\nYou Solved it! Typed: %s", wantedWord);
  for (size_t i = 0; i < MAX_SPELLER_WORD; i++)
  {
    currentWord[i] = 0;
  }
  HAL_Delay(500);
}

bool getKeyerWord(char* currentWord, const char* wantedWord, size_t size)
{
  bool firstCharHit = false;
  //lcd_goto_line(1, 0);
  bool checkAgainstWord = currentWord != NULL || wantedWord != NULL;

  int tipState = 0;
  int ring2State = 0;

  AllowedInputContact state = AllowedInputContact::Unrestricted;
  int currentTime = 0;

  WhiteSpaceState whiteSpaceState = WhiteSpaceState::StartOfWord;
  int lastTime = HAL_GetTick();
  unsigned int currentWordPosition = 0;
  while (true)
  {
    // Poll for pins so we wont just continue to delay dit length
    // Makes more responsive after time without user input
    do {
      currentTime = HAL_GetTick();
      if (whiteSpaceState == WhiteSpaceState::RecievedLetter && currentTime - lastTime >= longSignalLengthMS)
      {
        if (!firstCharHit)
        {
          firstCharHit = true;
          lcd_clear();
          HAL_Delay(5);
        }
        char currentChar = Translator.translate();
        printf("%c", currentChar);
        lcd_send_data(currentChar);
        if (checkAgainstWord)
        {
          if (wantedWord[currentWordPosition] == currentChar)
          {
            currentWord[currentWordPosition++] = currentChar;
          }
        }
        whiteSpaceState = WhiteSpaceState::ProcessedLetter;
      }
      if ( whiteSpaceState == WhiteSpaceState::ProcessedLetter && currentTime - lastTime >= (longSignalLengthMS + longSignalLengthMS + shortSignalLengthMS))
      {
        printf(" ");
        //lcd_send_data(' ');
        whiteSpaceState = WhiteSpaceState::StartOfWord;
        
        return (strcmp(currentWord, wantedWord) == 0);
      }
      
      tipState = HAL_GPIO_ReadPin(TIP_PIN_PORT, TIP_PIN);
      ring2State = HAL_GPIO_ReadPin(RING1_PIN_PORT, RING1_PIN);

    } while (tipState != GPIO_PIN_RESET && ring2State != GPIO_PIN_RESET);

    whiteSpaceState = WhiteSpaceState::RecievedLetter;


    // Check to see if we have dah pressed after dit was pressed.
    // Allows for oscillating between dit and dah when dah is first
    if (ring2State == GPIO_PIN_RESET && state == AllowedInputContact::AwaitingRing)
    {
      state = AllowedInputContact::AwaitingRing;
    }
    else
    {
      state = AllowedInputContact::Unrestricted;
    }
    
    if (tipState == GPIO_PIN_RESET && state != AllowedInputContact::AwaitingRing)
    {
      state = AllowedInputContact::AwaitingRing;
      
      SpeekerPlayer.playShort();
      Translator.addDot();
    }
    else if (ring2State == GPIO_PIN_RESET && state != AllowedInputContact::AwaitingTip)
    {
      state = AllowedInputContact::AwaitingTip;
      
      SpeekerPlayer.playLong();
      Translator.addDash();
    }

    lastTime = HAL_GetTick();
    HAL_Delay(shortSignalLengthMS);
  }
         
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSI
    | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON; 
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_PeriphCLKInitTypeDef RNGPeriphClkInit = {0};

  RNGPeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  RNGPeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&RNGPeriphClkInit) != HAL_OK)
  {
      Error_Handler();
  }

  RCC_PeriphCLKInitTypeDef I2C1PeriphClkInit = {0};

  I2C1PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  I2C1PeriphClkInit.RngClockSelection = RCC_I2C1CLKSOURCE_PCLK1;

  if (HAL_RCCEx_PeriphCLKConfig(&I2C1PeriphClkInit) != HAL_OK)
  {
      Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  __HAL_RCC_USART2_CLK_ENABLE();
  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_NVIC_SetPriority(USART2_IRQn, 1, 0); // Set priority level (1 is lower priority than 0)
  HAL_NVIC_EnableIRQ(USART2_IRQn);   
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */

  /*Configure GPIO pin : Buzzer*/
  GPIO_InitTypeDef buzzerInitStruct = {0};
  buzzerInitStruct.Pin = BUZZER_PIN;
  buzzerInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  buzzerInitStruct.Pull = GPIO_NOPULL;
  buzzerInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &buzzerInitStruct);

  /* Configure TIP_PIN and RING1_PIN as inputs with pull-up */
  GPIO_InitTypeDef tipInitStruct = {0};
  tipInitStruct.Pin = TIP_PIN;
  tipInitStruct.Mode = GPIO_MODE_INPUT;
  tipInitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TIP_PIN_PORT, &tipInitStruct);

  /* Configure TIP_PIN and RING1_PIN as inputs with pull-up */
  GPIO_InitTypeDef ringInitStruct = {0};
  ringInitStruct.Pin = RING1_PIN;
  ringInitStruct.Mode = GPIO_MODE_INPUT;
  ringInitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RING1_PIN_PORT, &ringInitStruct);

  /* Configure USART2 GPIO pins (PA2=TX, PA3=RX) */
  GPIO_InitTypeDef usartGPIOInitStruct = {0};
  usartGPIOInitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  usartGPIOInitStruct.Mode = GPIO_MODE_AF_OD;
  usartGPIOInitStruct.Pull = GPIO_NOPULL;
  usartGPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  usartGPIOInitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &usartGPIOInitStruct);

  /* Configure I2C1 pints (PA9=scl PA10=sda)*/
  GPIO_InitTypeDef i2cInitStruct = {0};
  i2cInitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  i2cInitStruct.Mode = GPIO_MODE_AF_OD;
  i2cInitStruct.Pull = GPIO_PULLUP;
  i2cInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  i2cInitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOA, &i2cInitStruct);

}

void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00707CB7;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.OwnAddress2 = 0;                           // ADD THIS
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // ADD THIS
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // ADD THIS
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     // ADD THIS
    
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)                   // ADD ERROR CHECKING
    {
        Error_Handler();
    }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
 {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
