package test.java;

import java.util.Arrays;                           // sort method.

/**
 * PerformanceUtilities class is intended to provide general support 
 * for measuring Aida's performance
 *
 * @author Greg White, 29/Aug/08
 */
public class PerformanceUtilities
{
	/**   
	 * Returns median of array of doubles
	 *
	 * @return median value of the given array
	 */
	protected static double median(double[] a) 
	{
		double [] m = (double []) a.clone();
		Arrays.sort(m);
		int middle = m.length/2;  // Subscript of middle element
		if (m.length%2 == 1)      // Odd num elements, return middle
			return m[middle];
		else                      // Even elements, return average of middle two
			return (m[middle-1] + m[middle]) / 2.0;
	}
}

