/***********************
 * Assignment 2
 * CSC354S
 * By: Sergei Iakhnin
 * 981 843 420
 *************************/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <string.h>





/* Compare function for qsort. */
int compare (const void * a, const void * b){
  return (int)((*(float*)a * 100) - (*(float*)b * 100));
}


int main(int argc, char** argv){
	/* Sample minimum, maximum, and mean. */
	double sample_min = DBL_MAX, sample_max = 0, sample_mean = 0;

	/* Sample median, variance, coefficient of variation, and skewness. */
	double sample_median, sample_variance, sample_cov, sample_skewness;

	/* Sample lexis ratio */
	double lexis_ratio;

	/* Midpoint of quartile, octile and extremes quantiles. */
	double quartile_midpoint, octile_midpoint, extremes_midpoint;

	/* MLE mu and sigma parameters for lognormal distribution. */
	double mle_mu = 0, mle_sigma = 0;
	
	/* If wrong parameters. */
	if(argc < 3){
		cout << "Incorrect number of parameters. Proper usage: a2 input_file num_intervals. Terminating. \n";
		cout.flush();
		exit(1);
	/* otherwise */
	}else{
		int i;
		
		/* get input file name */
		char* filename = argv[1];
		
		FILE *infile;
		
		/* open input file */
		if((infile = fopen(filename, "rw")) == NULL){
			cout << ": Error opening file " << infile << "\n";
			exit(1);
		}
		
		char delim;	
		int ctr = 0;
		int num_rec;
		
		/* Red in number of records */
		fscanf(infile, "%d", &num_rec);
		fgetc(infile);
		
		float* distr = (float*)malloc(num_rec * sizeof(float));
		float* order_stats = (float*)malloc(num_rec * sizeof(float));
		
		/* Read in data */
		while((delim = fgetc(infile)) != EOF){
			fscanf(infile, "%f", &(distr[ctr]));
			ctr++;
		}
		
		/* Retrieve number of histogram intervals from command line. */
		int num_intervals = atoi(argv[2]);
		
		/* Allocate memory for histogram. */
		int* histogram = (int*)malloc(num_intervals * sizeof(int));
	
		/* Zero in the histogram. */
		for(i = 0; i < num_intervals; i++){
			histogram[i] = 0;
		}

		/* Copy the sample data into the order_stats array. */
		memcpy(order_stats, distr, num_rec * sizeof(float));
		
		/* Sort order statistics using qsort. */
		qsort(order_stats, num_rec, sizeof(float), compare); 
		
		/* Calculate sample min and max. */
		sample_min = order_stats[0];
		sample_max = order_stats[num_rec - 1];
		
		/* Range of observations. */
		double range = sample_max - sample_min;
		
		/* Length of one histogram interval. */
		double delta = range / num_intervals;

		/* Go through all observations. */
		for(i = 0; i < num_rec; i++){
			
			/* Accumulate sample mean. */
			sample_mean += distr[i];
			
			/* Accumulate mu MLE. */
			mle_mu += log10(distr[i]);

			/* Accumulate histogram. */
			if(distr[i] == sample_max){
				histogram[num_intervals - 1]++;
			}else{
				/* Determine what interval current observation belongs to. */
				int ind = (int)floor((distr[i] - sample_min) / delta);

				/* Accumulate histogram. */
				histogram[ind]++;
			}

		}

		/* Calculate sample mean. */
		sample_mean /= num_rec;

		/* Calculate mu MLE */
		mle_mu /= num_rec;

		/* Accumulate sample variance, skewness and sigma MLE. */
		for(i = 0; i < num_rec; i++){
			sample_variance += pow((double)(distr[i] - sample_mean), 2);
			sample_skewness += pow((double)(distr[i] - sample_mean), 3) / num_rec;
			mle_sigma += pow(log10(distr[i]) - mle_mu, 2);
		}

		/* Calculate sample variance. */
		sample_variance /= num_rec - 1;

		/* Calculate sample skewness. */
		sample_skewness /= sqrt(pow(sample_variance, 3));

		/* Calculate sigma MLE. */
		mle_sigma /= num_rec;
		mle_sigma = sqrt(mle_sigma);

		/* Calculate sample coefficient of variation. */
		sample_cov = sqrt(sample_variance) / sample_mean;

		/* Calculate the lexis ratio. */
		lexis_ratio = sample_variance / sample_mean;
		
		int ind;

		/* Calculate sample median. */
		if(num_rec % 2){
			ind = (num_rec + 1) / 2;
			sample_median = order_stats[ind];
		}else{
			ind = num_rec / 2;
			sample_median = (order_stats[ind] + order_stats[ind + 1]) / 2;
		}
		
		/* Calculate quartile midpoint. */
		if(ind % 2){
			ind = (ind + 1) / 2;
			quartile_midpoint = (order_stats[ind] + order_stats[num_rec - ind]) / 2;

		}else{
			ind = (ind) / 2;
			quartile_midpoint = ((order_stats[ind] + order_stats[ind + 1]) / 2 + 
				(order_stats[num_rec - ind] + order_stats[num_rec - ind + 1]) / 2) / 2;

		}
		
		/* Calculate octile midpoint. */
		if(ind % 2){
			ind = (ind + 1) / 2;
			octile_midpoint = (order_stats[ind] + order_stats[num_rec - ind]) / 2;

		}else{
			ind = (ind) / 2;
			octile_midpoint = ((order_stats[ind] + order_stats[ind + 1]) / 2 + 
				(order_stats[num_rec - ind] + order_stats[num_rec - ind + 1]) / 2) / 2;

		}
		
		/* Calculate extremes midpoint. */
		extremes_midpoint = (order_stats[num_rec - 1] + order_stats[0]) / 2;

		cout << "Task 1a\n-----------------\n";
		
		/* Print summary statistics. */
		cout << "Summary Statistics\n\n";
		cout << "Sample Minimum: " << sample_min << "\n";
		cout << "Sample Maximum: " << sample_max << "\n";
		cout << "Sample Median: " << sample_median << "\n";
		cout << "Sample Mean: " << sample_mean << "\n";
		cout << "Sample Variance: " << sample_variance << "\n";
		cout << "Sample Coefficient of Variation: " << sample_cov << "\n";
		cout << "Lexis Ratio: " << lexis_ratio << "\n";
		cout << "Sample Skewness: " << sample_skewness << "\n";

		/* Print histogram. */
		cout << "\nThe histogram: \n\n";

		for(i = 0; i < num_intervals; i++){
			for(int j = 0; j < histogram[i]; j++){
				cout << "*";
			}
			cout << "\n";
		}
		cout << "\n";
		
		/* Print quantile summary */
		cout << "The quantile summary : \n\n";
		cout << "Median midpoint - " << sample_median << "\n";
		cout << "Quartile midpoint - " << quartile_midpoint << "\n";
		cout << "Octile midpoint - " << octile_midpoint << "\n";
		cout << "Extremes midpoint - " << extremes_midpoint << "\n";
		cout << "\n";


		/* Print the computed MLE's for the lognormal distribution. */
		cout << "\n\nTask 2, Lognormal MLEs for distribution.\n---------------------\n";
		cout << "MLE mu: " << mle_mu << "\n";
		cout << "MLE sigma: " << mle_sigma << "\n";
		
		double chi_square = 0, pSubJ = delta / (sample_max - sample_min);
		
		/* Compute chi-squared statistic */
		
		for(i = 0; i < num_intervals; i++){
			chi_square += (pow((histogram[i] - num_rec * pSubJ), 2) / (num_rec * pSubJ));
		}

		cout << "\nTask 3A, chi-squared statistic\n------------------------------\n\n";
		cout << "Chi squared value: " << chi_square << "\n";
	}
	return 1;
}

