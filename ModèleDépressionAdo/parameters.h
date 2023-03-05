#pragma once

#include <array>
#include <vector>
#include <string>

// Structure to hold parameters
template <class T> struct param
{
	T mean;
	T low;
	T high;
	int type;
};

template <class T> class parameters
{
public:
	parameters();
	parameters(std::string path, char c);
	parameters(std::vector<param<T>>& p);

	T operator[](size_t i) const;
	void set(int i, T val);
	
	void dsa(int param, bool low);
	void psa();
	void refresh_parameters();
	
	size_t getN() const;

private:
	

	
	// Vector of parameters
	std::vector<param<T>> p;

	// Vector of mean values
	std::vector<T> mean_p;

	// Initialize mean_p
	void init();

	// Size
	size_t n;

	T normal(double mean, double low);
	T lognormal(double mean, double low);
	T beta(double mean, double low);
	T gamma(double mean, double low);

	// Functions to read CVS into Vectors
	std::vector<T> read_data(std::string pathway, char c);


	
};


// Default constructor, fill the mean_param with p value
template <class T> parameters<T>::parameters() : n(0)
{
}

// Initialize mean_p
template <class T> void parameters<T>::init()
{
	if (p.size() == 0) return;

	n = p.size();

	for (size_t i = 0; i < n; i++)
	{
		mean_p.push_back(p[i].mean);
	}

	
};

// Construct the parameters from a CSV file
// The file needs to be structured with four colums and a separator c, default is comma
template <class T> parameters<T>::parameters(std::string path, char c) : parameters()
{
	read_data(path, c);
	init();
}

template <class T> parameters<T>::parameters(std::vector<param<T>>& ext_p) : p(ext_p)
{
	init();
}

template <class T> T parameters<T>::operator[](size_t i) const
{
	return i < n ? mean_p[i] : 0;
}

template <class T> void parameters<T>::parameters::set(int i, T val)
{
	if (i >= n) return;

	// Set the value for mean_p
	mean_p[i] = val;

}

template <class T> void parameters<T>::dsa(int param, bool low)
{
}

template <class T> void parameters<T>::psa()
{
	// Do all parameters
	for (int i = 0; i < n; i++)
	{
		//std::cout << i << ": " << p[i][0] << "-";
		switch (static_cast<int>(p[i][3]))
		{
		default:
			//std::cout << "default" << std::endl;
			break;
		case 0:
			// Do nothing;
			//std::cout << "0" << std::endl;
			break;
		case 1:
			mean_p[i] = normal(p[i][0], p[i][1]);
			//std::cout << "1" << std::endl;
			break;
		case 2:
			mean_p[i] = beta(p[i][0], p[i][1]);
			//std::cout << "2" << std::endl;
			break;
		case 3:
			mean_p[i] = gamma(p[i][0], p[i][1]);
			//std::cout << "3" << std::endl;
			break;
		case 4:
			mean_p[i] = lognormal(p[i][0], p[i][1]);
			//std::cout << "4" << std::endl;
			break;
		}
		//std::cout << p[i][0] << std::endl;
	}
}

template <class T> void parameters<T>::refresh_parameters()
{
	init();
}

template <class T> size_t parameters<T>::getN() const
{
	return n;
}

template <class T> T parameters<T>::normal(double mean, double low)
{
	/*return rnd_psa.normal(mean, abs(mean - low) / 1.96);*/
}

template <class T> T parameters<T>::lognormal(double mean, double low)
{
	/*double m(mean);
	double s(pow(abs(mean - low) / 1.96, 2));
	double alpha(log(m) - 0.5 * log(1 + pow(s / m, 2)));
	double beta(log(1 + pow(s / m, 2)));
	return rnd_psa.lognormal(alpha, beta);*/
}

template <class T> T parameters<T>::beta(double mean, double low)
{
	/*double m(mean);
	double s(pow(abs(mean - low) / 1.96, 2));
	double alpha = -m*(s + m * m - m) / s;
	double beta = (s + m * m - m) * (m - 1) / s;
	return rnd_psa.betad(alpha, beta);*/

}

template <class T> T parameters<T>::gamma(double mean, double low)
{
	/*double m(mean);
	double s(pow(abs(mean - low) / 1.96, 2));
	double alpha = pow(m,2)/s;
	double beta = s/m;
	return rnd_psa.gamma(alpha, beta);*/
}

template <class T> std::vector<T> parameters<T>::read_data(std::string path, char c)
{

	/*std::ifstream myFile;
	myFile.open(path);
	std::string line, field;
	std::vector<std::vector<param>> array{}; //the 2d array
	param v; //the 1d array to store each row

	while (getline(myFile, line))
	{
		v.clear(); //clear it before filling it again
		std::stringstream ss(line);

		while (std::getline(ss, field, c))
		{
			std::stringstream ss2(field);
			if (ss2 >> donnee)
			{
				v.push_back(donnee);
			}
		}
		array.push_back(v);
	}
	myFile.close();

	return array;*/
}
