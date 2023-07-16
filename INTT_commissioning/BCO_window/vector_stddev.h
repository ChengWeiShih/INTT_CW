double  vector_average (vector <double> input_vector)
{
	double sum=0;
	double result;
	for (int i=0; i<input_vector.size(); i++)
		{
			sum+=input_vector[i];

		}
	result=sum/input_vector.size();	
	return result;
}

double vector_stddev (vector <double> input_vector)
{
	double sum=0;
	double average = 0;
	double sum_subt = 0;
	for (int i=0; i<input_vector.size(); i++)
		{
			sum+=input_vector[i];

		}
	average=sum/input_vector.size();
	// cout<<"average is : "<<average<<endl;

	for (int i=0; i<input_vector.size(); i++)
		{
			//cout<<input_vector[i]-average<<endl;
			sum_subt+=pow((input_vector[i]-average),2);

		}
	//cout<<"sum_subt : "<<sum_subt<<endl;
	double stddev;
	stddev=sqrt(sum_subt/(input_vector.size()-1));	

	// cout<<"print from the function, average : "<<average<<" std : "<<stddev<<endl;

	return stddev;
}	