# Aquaponic-project

The problem of this study is the low capability of current farming system to overcome the global hunger. The specific problem proposed for research was to overcome the high demand of food production, using limited resources such as land, time and cost. This quantitative study will examine the improvement of farming system using technology such as sensor, actuator, microcontroller, and IoT as well as mutualism farming system which was aquaponics system.


i)	To develop an improvement for the aquaponics system using microcontroller, sensor and actuators

ii)	To examine the result of the improved aquaponics system during the demonstration.

The aquaponic project inspired based on similar structure on the figure below with additional feature of IoT monitoring system.

![image](https://user-images.githubusercontent.com/43923087/129076194-51fc6780-3593-49bb-b178-11ba40bcb403.png)

# Overview of the framework 

![image](https://user-images.githubusercontent.com/43923087/129078063-0e27b378-3dff-46e6-b4d2-d32069a2c5e7.png)


The big picture of the systems was shown in the figure above, where the sensors and actuators were interconnected to photon particle, whereby the data transfer from Arduino to photon and the response will be recorded at the particle photon by publishing the data into the cloud.

Node-red then was established from Raspberry pi under a local host server to be a medium platform for micro-controllers to transfer and access the data. 
The data from the photon particle will then be fetched by Node-red with displaying into dashboard via Node-red feature library. 

Dashboard then was further improved from hosting a local server to online server using port-forward from the local router. 
The Dashboard feature in local host server allows devices to access freely, covered that the device is connected to the same router. 
Dashboard in local host has the capability of real-time data flow, this feature can be further enhanced in terms of grinding data for varieties purposes. 
Dashboard enhancement can be branched into many useful data, such as prediction of seasonal temperature, growth rate of the crops and many other possibilities. 
Connections to the sensors and actuators 

![image](https://user-images.githubusercontent.com/43923087/129076206-4ab0865e-e9f1-42d8-9bdd-488fd299bb3b.png)


Full on schematic diagram

![image](https://user-images.githubusercontent.com/43923087/129077157-e80b0315-5c51-4064-802c-d0c2ffd9ffdc.png)


The sample aquaponic prototype 

![image](https://user-images.githubusercontent.com/43923087/129076859-c88e2f1b-bb62-43eb-864a-ba30b98cad92.png)


![image](https://user-images.githubusercontent.com/43923087/129080127-5ab414a2-eadb-4eda-94f3-45cd2a3ef230.png)

