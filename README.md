# Kidnapped Vehicle project – Project 3

## Overview:

This project implements a Particle filter Algorithm in C++ and tested on Simulator provided by Udacity. The communication between the simulator and the EKF is done using WebSocket using the uWebSockets implementation on the EKF side. The Simulator also provides the end result of whether Particle filter algorithm was able to meet the project criteria.


This project utilizes the starter code given by Udacity. Following are the outputs available as outcome of this project:

1. Vide0.mp4 – Contains recorded video of the output in the simulator
2. Cmake-output.log- Contains output of Cmake
3. make-output.log - contains output of make

## Environment:

This project was done in Ubuntu 18 LTS and following are the components
• cmake >= 3.5
• make >= 4.1
• gcc/g++ >= 5.4

## Ouput:

Below are the output of this project:

### cmake output:

<img src=https://github.com/rameshbaboov/CarND-Kidnapped-Vehicle-Project/blob/master/img/1.JPG width = "480">

### make output:


### Simulator output:

Once the code is run following message is displayed. The application connects to the Simulator on port # 4567 , once the project-3 is seleted and started on simulator as shown below:



### Code:

Particle_filter.cpp – I used the starter code provided by Udacity. Below are the functions of this code:

1. ParticleFilter::init - Initiates the no of particles to 100. add random gaussian noise to each of the particle for x, y and theta.

2. ParticleFilter::prediction: adds measurements to each particles and the random noise. The following equation is used:
if yaw rate is zero:

3. ParticleFilter::dataAssociation - Finds the predicted measurement that is closest to each
observed measurement and assign the observed measurement to this particular landmark. Called by
ParticleFilter::updateWeights. This uses the dist function to calculate the distance and selects the
shortest distance

4. ParticleFilter::updateWeights – calculates the weights of each particle using a multvariate
Gaussian distribution. Also landmarks that are within the range are only considered because
they only would hae been detected ny the partile



## RUBRICS:
This project satisfies all the rubric points

### Rubric#1: Accuracy

### Criteria                                                              Meets Specifications
Does your particle filter                                   This criteria is checked automatically when you do ./run.sh in the
localize the vehicle to within                              terminal. If the output says "Success! Your particle filter passed!"
the desired accuracy?                                       then it means you’ve met this criteria.

Yes. The output says - "Success! Your particle filter passed!"


### Rubric#2: Performance

### Criteria                                                  Meets Specifications

Does your particle run                                        This criteria is checked automatically when you do ./run.sh in the
within the specified time of                                  terminal. If the output says "Success! Your particle filter passed!" 100 seconds?                                         then it means you’ve met this criteria

Yes. The output says - "Success! Your particle filter passed!". It runs within 50 seconds.

### Rubric#3: General

### Criteria                                     Meets Specifications

Does your code use a                             There may be ways to “beat” the automatic grader without actually
particle filter to localize                      implementing the full particle filter. You will meet this criteria if the
the robot?


Particle filter works as expected




