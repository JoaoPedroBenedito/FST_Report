#include "xparameters.h"
#include "xgpio.h"

//This is a macro that gets my GPIO ID from the parameters file xparameters.h
#define MY_GPIO_ID XPAR_LEDS_OUTPUT_GPIO_DEVICE_ID

//This is a delay between updates in the leds, or we will not be able to visualize it
#define LED_DELAY     1000000

//following constant is used to determine which channel of the GPIO is
//used if there are 2 channels supported in the GPIO.
#define LED_CHANNEL 1

//This is the width we set for the GPIO */
#define GPIO_BITWIDTH   8

//FSL related macros
#define input_slot_id   XPAR_FSL_FSL_EXAMPLE_0_INPUT_SLOT_ID
#define output_slot_id  XPAR_FSL_FSL_EXAMPLE_0_OUTPUT_SLOT_ID

#include "fsl.h" 
#define write_into_fsl(val, id)  putfsl(val, id)
#define read_from_fsl(val, id)  getfsl(val, id)

//FSL related macros
#define camerain_slot_id   1
#define cameraout_slot_id  1

/*number of lines in the frame*/
#define num_lin (64)
/*number of columns in the frame*/
#define num_col (128)
/*pixel address macro*/
#define pixel(a,b) (image[a * num_lin + b])
/*number of pixels in a frame*/
#define N (num_lin*num_col)

/*image buffer*/
unsigned char image[N];

/*reads a frame to the image buffer*/
void read_image()
{
   int n;

    for (n = 0 ; n < N ; n++) {
		microblaze_bread_datafsl(image[n], cameraout_slot_id);
	}
}

/*displays the image buffer in the VGA monitor*/
void write_image()
{
	int n;
	for (n = 0 ; n < N  ; n++) {		
		microblaze_bwrite_datafsl(image[n],camerain_slot_id);
	}
}


int main(void)
{
	//this variable will contain the value presented in the leds
	unsigned int OutData;
	//this is the driver for our GPIO
	XGpio GpioDrv;
	//delay control variable
	int DelayCtl;
	
	int i;
	 
	 //Initialize the GPIO driver so that it's ready to use,
	 //specify the device ID that is generated in xparameters.h
	 XGpio_Initialize(&GpioDrv, MY_GPIO_ID);


    //Set the direction for all signals to be outputs
    XGpio_SetDataDirection(&GpioDrv, LED_CHANNEL, 0x0);

	//we will perform a rotacional shift of the light in the leds
	OutData = 0x01;
	while(1)
	{
		/*if(OutData & 0x80)
			OutData = 0x01;
		else
			OutData <<= 1;*/
			
		write_into_fsl(OutData, input_slot_id);
		read_from_fsl(OutData, output_slot_id);
			
		//write to the correct channel of the GPIO
		XGpio_DiscreteWrite(&GpioDrv, LED_CHANNEL, OutData);
	
		//apply the delay before next update
		for(DelayCtl = 0; DelayCtl < LED_DELAY; DelayCtl++);
		
		/*read a full frame from the camera*/
		read_image();

		/*image negative*/
		for (i = 0; i < N; i++)
		    image[i] = 		255 - image[i];

		/*write a full frame to the VGA ouput*/
		write_image();
		
	}

    return 0;
}
