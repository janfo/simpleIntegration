# Are you a student or an engineer? Wanna calculate integral? Try simpleIntegration!

Example nr 1:

Calculate the value of the following integral. \
![Zrzut ekranu 2025-01-13 200428](https://github.com/user-attachments/assets/2bf008f9-7d10-4455-9320-30f07bde88fa)

```
#include <iostream>
#include "Integrator.h"
#define NR_OF_DISC_PTS 1500
int main()
{

	auto fun = std::function<float(float)>([=](float x)  -> float {return (x); });
	Integrator <float, float> Integ(fun, NR_OF_DISC_PTS, { 0, 2});
	std::cout<< "Integral value: "<< Integ.integrate() << std::endl;
	
	return 0;
}
```


Example nr 2:

Calculate double integral \
![Zrzut ekranu 2025-01-13 202734](https://github.com/user-attachments/assets/3866ca08-d771-4849-953f-46c6320076dd)

> [!NOTE]
> The analytical solution of the integral is 4pi

```
#include <iostream>
#include "Integrator.h"

#define NR_OF_DISC_PTS 1500

int main()
{

	auto fun = std::function<float(float, float)>([=](float theta, float phi)  -> float {return (sin(theta)); });
	Integrator <float, float, float> Integ(fun, NR_OF_DISC_PTS, { 0, 3.14f, 0, 3.14f * 2.0f });
	std::cout << "Integral value: " << Integ.integrate() << std::endl;
	
	return 0;
}

```

Example nr 3

Calculate complex integral \
![Zrzut ekranu 2025-01-13 204424](https://github.com/user-attachments/assets/77c56000-1c15-4cc7-8eec-804678239c34)

```
#include <iostream>
#include "Integrator.h"
#include <complex>
#define NR_OF_DISC_PTS 1500


using namespace std::complex_literals;

int main()
{
	auto fun = std::function<std::complex<double>(std::complex<double>)>([=](std::complex<double> x)  -> std::complex<double> {return std::exp(1i*x); });
	Integrator <std::complex<double>, std::complex<double>> Integ(fun, NR_OF_DISC_PTS, { 0, 3.14});
	std::cout << "Integral value: " << Integ.integrate() << std::endl;
	
	return 0;
}

```
