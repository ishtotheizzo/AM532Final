# AM532Final
IMU based real time art


# Tools 
## Sensor 

M5 Stick Plus: https://www.digikey.com/en/product-highlight/m/m5stack/m5stickc-plus-development-kit?utm_adgroup=xGeneral&utm_source=google&utm_medium=cpc&utm_campaign=Dynamic%20Search_EN_Product&utm_term=&utm_content=xGeneral&gclid=CjwKCAiAhreNBhAYEiwAFGGKPAwhraiH_pqDUnkfJ2bjzJIGx-uyTP786j9Uai9qY_cV-CuKY_4ZTxoCw-YQAvD_BwE 


## Software 

Arduino:   Platform used for M5 Stick <br> 

MAX MSP: Used to generate the real time data 

# This Repo 

1. M5Stick.ino: this is the Arduino code. There has been no magnometer soldered onto this device (you should add the magnometer to help transform into quarternians). 
2. max/express.maxpro - the Max MSP project. This project hosts the node server that this project runs on 
