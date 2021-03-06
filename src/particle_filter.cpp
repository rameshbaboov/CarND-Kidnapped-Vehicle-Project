/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include "particle_filter.h"

using namespace std;

const double near_zero = 0.0001;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
	// TODO: Set the number of particles. Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS) and all weights to 1. 
	// Add random Gaussian noise to each particle.
	// NOTE: Consult particle_filter.h for more information about this method (and others in this file).
	
	
	default_random_engine gen;
	num_particles = 100;
	
	// define normal distributions for sensor noise
  	normal_distribution<double> xnoise(0, std[0]);
	normal_distribution<double> ynoise(0, std[1]);
	normal_distribution<double> thetanoise(0, std[2]);

	//initialize all particles

	for (int i = 0 ; i < num_particles; i++) 
	
	{

		Particle p;  //struct from particle_filter.h
		p.id = 1;
		p.x = x;
		p.y = y;
		p.theta = theta;
		p.weight = 1;
		

		// add noise to particle
		p.x += xnoise(gen);
		p.y += ynoise(gen);
		p.theta += thetanoise(gen);

		// move the temporary values to actual particle vector
		particles.push_back(p);
	
	}
	
	is_initialized = true;
	cout << "Particle filter instance created " << endl;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
	// TODO: Add measurements to each particle and add random Gaussian noise.
	// NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	//  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	//  http://www.cplusplus.com/reference/random/default_random_engine/

	default_random_engine gen;

	double std_x = std_pos[0];
	double std_y = std_pos[1];
	double std_theta = std_pos[2];

	cout << "running prediction" << endl;
	// Create a normal (Gaussian) distribution for x
	normal_distribution<double> dist_x(0, std_x);
	
	// TODO: Create normal distributions for y and theta
	normal_distribution<double> dist_y(0, std_y);
	normal_distribution<double> dist_theta(0, std_theta);
	
	//add measurements to particles
	for (int i = 0 ; i < num_particles; i++)
	
	 {

		// check if yaw rate is zero as there are two different equations for both zero and non zero
		if (fabs(yaw_rate) < near_zero) 
		{
			particles[i].x += velocity * delta_t * cos(particles[i].theta);
			particles[i].y += velocity * delta_t * sin(particles[i].theta);
		}
		
		else
		{
			particles[i].x += velocity / yaw_rate * (sin(particles[i].theta + yaw_rate*delta_t) - sin(particles[i].theta));
      			particles[i].y += velocity / yaw_rate * (cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*delta_t));
			particles[i].theta += yaw_rate * delta_t;
		}
		 // add noise
		particles[i].x += dist_x(gen);
		particles[i].y += dist_y(gen);
		particles[i].theta += dist_theta(gen);
	 }
	

}

void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.
	// NOTE: this method will NOT be called by the grading code. But you will probably find it useful to 
	//   implement this method and use it as a helper during the updateWeights phase.

	
	for (unsigned int i = 0; i < observations.size(); i++) 
	{
		//get current obervation
		LandmarkObs obs = observations[i];

		//initialize minimum distance to the max poss dist
		double min_dist = numeric_limits<double>::max();

		//initialize landmark id from map placeholder with observation
		int map_id = -1;

		// search for all the land mark and return id of that landmark which is nearer to the observation
		for (unsigned int j = 0 ; j < predicted.size(); j++) 
		{
			//get current predictoon
			LandmarkObs pred = predicted[j];

			// get distance between current and predicted landmarks
			double cur_dist = dist(obs.x, obs.y, pred.x, pred.y);

			// find the predicted landmark nearest to the current observed landmark
     			 if (cur_dist < min_dist) {
      				min_dist = cur_dist;
        			map_id = pred.id;
			   }
		}

		// set the observation id to the nearest predicted landmark id
		observations[i].id = map_id;

	}
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		const std::vector<LandmarkObs> &observations, const Map &map_landmarks) {
	// TODO: Update the weights of each particle using a mult-variate Gaussian distribution. You can read
	//   more about this distribution here: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
	// NOTE: The observations are given in the VEHICLE'S coordinate system. Your particles are located
	//   according to the MAP'S coordinate system. You will need to transform between the two systems.
	//   Keep in mind that this transformation requires both rotation AND translation (but no scaling).
	//   The following is a good resource for the theory:
	//   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
	//   and the following is a good resource for the actual equation to implement (look at equation 
	//   3.33
	//   http://planning.cs.uiuc.edu/node99.html

	cout << "updatng weights" << endl;

	for (int i = 0; i < num_particles; i ++) 
	{
		// get the particle x, y coordinates
    		double p_x = particles[i].x;
		double p_y = particles[i].y;
		double p_theta = particles[i].theta;

		// create a vector to hold the map landmark 
		vector<LandmarkObs> predictions;

		for (unsigned int j = 0 ; j < map_landmarks.landmark_list.size(); j ++) 
		{
			//get the positions in a local variable
			double landmark_x = map_landmarks.landmark_list[j].x_f;
			double landmark_y = map_landmarks.landmark_list[j].y_f;
			int landmark_id = map_landmarks.landmark_list[j].id_i;

			//filter only for landmarks within sensor range from particle- since sensor range is limited, only those
			// landmark that are within the range would hae been detected ny the partile
			if (fabs(landmark_x - p_x)  <= sensor_range && fabs(landmark_y - p_y) <= sensor_range)
			{
				predictions.push_back(LandmarkObs{landmark_id, landmark_x,landmark_y});
			}

		}

		//create a list of observations that is transformed from vehicle cooardintes to map co ordinates
		vector<LandmarkObs> rotated_obs;
		for (unsigned int j = 0 ; j < observations.size(); j++)
		{
			double rotated_x = cos(p_theta)*observations[j].x - sin(p_theta)*observations[j].y +p_x;
			double rotated_y = sin(p_theta)*observations[j].x + cos(p_theta)*observations[j].y +p_y;
			rotated_obs.push_back(LandmarkObs {observations[j].id, rotated_x,rotated_y});
				
		}						
			
	
		dataAssociation(predictions,rotated_obs);

		//reset weight
		particles[i].weight = 1.0;

		
		for (unsigned j = 0; j < rotated_obs.size(); j++) 
		{
			//extract observations and coordinates
			double temp_x,temp_y,pred_x,pred_y;
			temp_x = rotated_obs[j].x;
			temp_y = rotated_obs[j].y;
			
			int temp_id = rotated_obs[j].id;

			// retrieve the x,y coordinates of the prediction associated with the current co ordinates
			for (unsigned int k = 0;  k < predictions.size(); k++) 
			{
				if (predictions[k].id == temp_id) {
				   pred_x = predictions[k].x;
				   pred_y = predictions[k].y; 	
			}

		}		

		//calculate weight for this observation with multivariate gaussian	
		double std_x = std_landmark[0];
		double std_y = std_landmark[1];
		double gauss_norm = (1/(2*M_PI*std_x*std_y));
		double termx, termy;
		termx = pow(pred_x - temp_x,2)/(2*pow(std_x,2));
		termy = pow(pred_y - temp_y,2)/(2*pow(std_y,2));
		double obs_weight = gauss_norm * exp(-(termx+termy));
		
		particles[i].weight *= obs_weight;
		
	}

	}

}

void ParticleFilter::resample() {
	// TODO: Resample particles with replacement with probability proportional to their weight. 
	// NOTE: You may find std::discrete_distribution helpful here.
	//   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution

	cout << "resampling" << endl;
	vector<Particle> resampled_particles;
	default_random_engine gen;
	vector<double> weights;
	double maxweight = numeric_limits<double>::min();

	for ( int i = 0; i <num_particles; i++) {
		weights.push_back(particles[i].weight);
		if (particles[i].weight > maxweight) 
		{
			maxweight = particles[i].weight;

		}
	}

	
	// create distribution
	uniform_real_distribution<double> distDouble(0.0, maxweight);
	uniform_int_distribution<int> distInt(0, num_particles - 1);

	
  	// generate index.
	 int index = distInt(gen);

	 double beta = 0.0;

	// rotate resampling wheel
 	
 	 for(int i = 0; i < num_particles; i++) {
  		  beta += distDouble(gen) * 2.0;
  		  while( beta > weights[index]) {
    		  beta -= weights[index];
   		   index = (index + 1) % num_particles;
  		  }
   		 resampled_particles.push_back(particles[index]);
  		}

	particles = resampled_particles;


	cout << "resampled" << endl;
}



Particle ParticleFilter::SetAssociations(Particle& particle, const std::vector<int>& associations, 
                                     const std::vector<double>& sense_x, const std::vector<double>& sense_y)
{
    //particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
    // associations: The landmark id that goes along with each listed association
    // sense_x: the associations x mapping already converted to world coordinates
    // sense_y: the associations y mapping already converted to world coordinates

    particle.associations= associations;
    particle.sense_x = sense_x;
    particle.sense_y = sense_y;

    return particle;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
