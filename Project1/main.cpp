#include <iostream>
#include <string>
#include <math.h>
using namespace std;

//School Method for Integer Addition
string addition(string num1, string num2, int base)
{
	int len1 = num1.size();
	int len2 = num2.size();
	int len3 = max(len1, len2) + 1;

	string result;

	//add 0s to make length equal
	if (len1 < len2)
	{
		for (int i = 0; i < len2 - len1; i++)
		{
			num1 = '0' + num1;
		}
		len1 = len2;
	}
	else if (len1 > len2)
	{
		for (int i = 0; i < len1 - len2; i++)
		{
			num2 = '0' + num2;
		}
		len2 = len1;
	}

	//addition
	int carry = 0;

	for (int i = 0; i < len3 - 1; i++)
	{
		int n1, n2, r;
		n1 = num1[len1 - 1 - i] - '0';
		n2 = num2[len2 - 1 - i] - '0';
		r = n1 + n2 + carry;

		int r1 = r;
		if (r >= base)
		{
			carry = 1;
			r1 = r - base;
		}
		else
		{
			carry = 0;
		}

		result = to_string(r1) + result;
	}
	result = to_string(carry) + result;

	//remove 0s
	int len = result.size();
	for (int i = 0; i < len; i++)
	{
		if (result[i] != '0')
		{
			len = i;
			break;
		}
	}

	result = result.substr(len, result.size() - len);

	if (result.size() == 0)
	{
		return "0";
	}
	
	return result;
}

//Subtraction
string subtraction(string num1, string num2, int base)
{
	int len1 = num1.size();
	int len2 = num2.size();

	//add 0s to make length equal
	while (len1 < len2) 
	{ 
		num1 = '0' + num1; len1++; 
	}
	while (len2 < len1) 
	{ 
		num2 = '0' + num2; len2++; 
	}

	string result;
	int borrow = 0;

	for (int i = len1 - 1; i >= 0; i--)
	{
		int n1 = num1[i] - '0';
		int n2 = num2[i] - '0' + borrow;

		if (n1 < n2)
		{
			n1 += base;
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}

		int r = n1 - n2;
		result = to_string(r) + result;
	}

	//remove leading zeros
	int index = 0;
	while (result[index] == '0' && index < result.size() - 1)
		index++;
	result = result.substr(index);

	return result;
}

//Karatsuba Algorithm for Integer Multiplication
string karatsuba(string num1, string num2, int base)
{
	int len1 = num1.size();
	int len2 = num2.size();

	if (len1 == 1 && len2 == 1)
	{
		int product = (num1[0] - '0') * (num2[0] - '0');
		string result;
		while (product > 0)
		{
			result = to_string(product % base) + result;
			product /= base;
		}
		return result.empty() ? "0" : result;
	}
	
	//add 0s to make length equal
	if (len1 < len2)
	{
		while (num1.size() < len2) num1 = '0' + num1;
		len1 = len2;
	}
	else if (len1 > len2)
	{
		while (num2.size() < len1) num2 = '0' + num2;
		len2 = len1;
	}

	int k = max(len1, len2) / 2;
	string high1 = num1.substr(0, len1 - k);
	string low1 = num1.substr(len1 - k);
	string high2 = num2.substr(0, len2 - k);
	string low2 = num2.substr(len2 - k);

	//recursion
	string z0 = karatsuba(low1, low2, base);
	string z2 = karatsuba(high1, high2, base);
	string z1 = karatsuba(addition(high1, low1, base), addition(high2, low2, base), base);
	z1 = subtraction(z1, z0, base);
	z1 = subtraction(z1, z2, base);

	for (int i = 0; i < 2 * k; i++)
	{
		z2 = addition(z2 + "0", "0", base);
	}
	for (int i = 0; i < k; i++)
	{
		z1 = addition(z1 + "0", "0", base);
	}

	//final addition
	string result = addition(z2, z1, base);
	result = addition(result, z0, base);

	//removing leading zeroes
	int index = 0;
	while (result[index] == '0' && index < result.size() - 1) 
		index++;
	result = result.substr(index);

	return result;
}

//Integer Division
string division(string num1, string num2, int base)
{
	if (num2 == "0") 
		return "Division by zero error";
	if (num1 == "0") 
		return "0";

	string result = "";
	string current = "";
	int i = 0;

	while (i < num1.size()) 
	{
		//add the next digit to the current part
		current += num1[i++];

		//remove leading zeros
		while (current.size() > 1 && current[0] == '0') 
		{
			current = current.substr(1);
		}

		int digit = 0;
		//subtract the divisor as many times as possible
		while (current.size() > num2.size() || (current.size() == num2.size() && current >= num2)) 
		{
			current = subtraction(current, num2, base);
			digit++;
		}

		//add the digit to the result
		result += to_string(digit);
	}

	//remove leading zeros
	while (result.size() > 1 && result[0] == '0') 
	{
		result = result.substr(1);
	}

	return result;
}

int main(void)
{
	string I1, I2, B;
	cout << "Enter three integers: ";
	cin >> I1 >> I2 >> B;

	int base = stoi(B);

	cout << addition(I1, I2, base) << " " << karatsuba(I1, I2, base) << " " << division(I1, I2, base) << endl;


	return 0;
}