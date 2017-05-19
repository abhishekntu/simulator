echo "----------------------------------------------"
echo "Soft Coarse Grain Overlay Architecture Simulator"
echo "----------------------------------------------"
echo "Virtualization Project - Arch Research Group NTU"
echo "----------------------------------------------"
echo "Starting Simulation"
echo "----------------------------------------------"
echo "Cleaning Build and Results Directory"
echo "----------------------------------------------"
make clean
echo "----------------------------------------------"
echo "Compiling Application"
echo "----------------------------------------------"
make
echo "----------------------------------------------"
echo "Running Application"
echo "----------------------------------------------"
./prodcons
echo "----------------------------------------------"
echo "Log Data for Processing Elements"
echo "----------------------------------------------"
echo "PE 0"
echo "----------------------------------------------"
cat Simulation_Results/PE_0.txt
echo "----------------------------------------------"
echo "PE 1"
echo "----------------------------------------------"
cat Simulation_Results/PE_1.txt
echo "----------------------------------------------"
echo "PE 2"
echo "----------------------------------------------"
cat Simulation_Results/PE_2.txt
