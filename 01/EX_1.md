## Exercise 1)

- How is the speedup of a parallel program defined?
		"the overall performance improvement gained by optimizing 
		 a single part of a system is limited by the fraction of time 
		 that the improved part is actually used" 
		 Citation: [https://en.wikipedia.org/wiki/Amdahl%27s_law]

- What is the formal definition of Amdahl's law and what relationship 
  does it describe for parallel programs (explain in your own words)? 
  Why/How is this significant?
		The Speedup of the Program S = p / (1 + (p - 1) / f) 
		   where p is the number of processors the Program can use and f is the fraction of the runtime the programm takes that is a serial section.

		A Parrallel Program can never run faster than the non-parralizable Parts and 
		adding more Resources speeds the program up linearly with how much of the Program is parralizable.

- Compute the theoretical speedup of a program that spends 10% 
  of its time in unparallelizable, sequential regions for 6 cores 
  and for a hypothetically unlimited number of cores. 
     
	 Six Cores:\
		Formula 1:

		S = 6 / (1 + (6 - 1) * 0.1) = 6 / 1.5 = 4
	 Unlimited Cores:\
		Formula 2:

		S = lim p -> ∞ (1 / (0.1 + (1 - 0.1) / p)) = 1 / (0.1 + (1 - 0.1) / ∞) = 1 / (0.1 + 0) = 10

- Compute the theoretical speedup of a program that spends 20% 
  of its time in unparallelizable, sequential regions for 6 cores 
  and for a hypothetically unlimited number of cores.
	
	 Six Cores:

		S = 6 / (1 + (6 - 1) * 0.2) = 6 / 2 = 3
	 Unlimited Cores:

		S = lim p -> ∞ (1 / (0.2 + (1 - 0.2) / p)) = 1 / (0.2 + (1 - 0.2) / ∞) = 1 / (0.2 + 0) = 5
		S = 1 / ((1 - 0.8) + (0.8 / Inf)) = 1 / (0.2 + 0) = 1 / 0.2 = 5

- Given an algorithm of time complexity O(n^3). How large (in %) 
  can the unparallelizable, sequential region be at most, such that 
  a speedup of 10 can be achieved using 64 cores?

		S(x) = 64 / (1 + (64 - 1) * x) = 10
		  <=> 10 = 64 / (1 + (64 - 1) * x)
		  <=> 1 + (64 - 1) * x = 64 / 10
		  <=> 63x = 6.4 - 1
		  <=> x = 5.4 / 63
		  <=> x = 0.0857

		The unparralizable region can be up to approximately 8.57%.

Formula 1: S = p / (1 + (p - 1) * f)\
Formula 2: S = 1 / (f + (1 - f) / p)