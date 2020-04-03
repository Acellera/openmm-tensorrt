#ifndef CUDA_NEURAL_NETWORK_KERNELS_H_
#define CUDA_NEURAL_NETWORK_KERNELS_H_

#include "NeuralNetworkKernels.h"
#include "openmm/cuda/CudaContext.h"
#include "openmm/cuda/CudaArray.h"

namespace NNPlugin {

/**
 * This kernel is invoked by NeuralNetworkForce to calculate the forces acting on the system and the energy of the system.
 */
class CudaCalcNeuralNetworkForceKernel : public CalcNeuralNetworkForceKernel {
public:
    CudaCalcNeuralNetworkForceKernel(std::string name, const OpenMM::Platform& platform, OpenMM::CudaContext& cu) :
            CalcNeuralNetworkForceKernel(name, platform), hasInitializedKernel(false), cu(cu),
            positionsTensor(NULL), boxVectorsTensor(NULL) {
    }
    ~CudaCalcNeuralNetworkForceKernel();
    /**
     * Initialize the kernel.
     * 
     * @param system         the System this kernel will be applied to
     * @param force          the NeuralNetworkForce this kernel will be used for
     * @param session        the TensorFlow session in which to do calculations
     * @param graph          the TensorFlow graph to use for computing forces and energy
     */
    void initialize(const OpenMM::System& system, const NeuralNetworkForce& force, TF_Session* session, TF_Graph* graph);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(OpenMM::ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    bool hasInitializedKernel;
    OpenMM::CudaContext& cu;
    TF_Session* session;
    TF_Graph* graph;
    TF_Tensor* positionsTensor;
    TF_Tensor* boxVectorsTensor;
    bool usePeriodic;
    OpenMM::CudaArray networkForces;
    CUfunction addForcesKernel;
};

} // namespace NNPlugin

#endif /*CUDA_NEURAL_NETWORK_KERNELS_H_*/
