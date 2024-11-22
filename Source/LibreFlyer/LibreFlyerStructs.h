#pragma once

#include "PhysicsSettingsEnums.h"
#include "LibreFlyerStructs.generated.h"

USTRUCT(BlueprintType)
struct LIBREFLYER_API FSimulationConfiguration {
	GENERATED_BODY()
public:
	/** Max Physics Delta Time to be clamped. */
	UPROPERTY(config, EditAnywhere, meta = (ClampMin = "0.0013", UIMin = "0.0013", ClampMax = "1.0", UIMax = "1.0"), Category = Framerate)
	float MaxPhysicsDeltaTime;
	/** Whether to substep the physics simulation. This feature is still experimental. Certain functionality might not work correctly*/
	UPROPERTY(config, EditAnywhere, Category = Framerate)
	bool bSubstepping;
	/** Whether to substep the async physics simulation. This feature is still experimental. Certain functionality might not work correctly*/
	UPROPERTY(config, EditAnywhere, Category = Framerate)
	bool bSubsteppingAsync;
	/** Max delta time (in seconds) for an individual simulation substep. */
	UPROPERTY(config, EditAnywhere, meta = (ClampMin = "0.0013", UIMin = "0.0013", ClampMax = "1.0", UIMax = "1.0", editcondition = "bSubStepping"), Category = Framerate)
	float MaxSubstepDeltaTime;
	/** Max number of substeps for physics simulation. */
	UPROPERTY(config, EditAnywhere, meta = (ClampMin = "1", UIMin = "1", ClampMax = "16", UIMax = "16", editcondition = "bSubstepping"), Category = Framerate)
	int32 MaxSubsteps;
	/** Physics delta time smoothing factor for sync scene. */
	UPROPERTY(config, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"), Category = Framerate)
	float SyncSceneSmoothingFactor;
	/** Physics delta time initial average. */
	UPROPERTY(config, EditAnywhere, meta = (ClampMin = "0.0013", UIMin = "0.0013", ClampMax = "1.0", UIMax = "1.0"), Category = Framerate)
	float InitialAverageFrameRate;
	/** The number of frames it takes to rebuild the PhysX scene query AABB tree. The bigger the number, the smaller fetchResults takes per frame, but the more the tree deteriorates until a new tree is built */
	UPROPERTY(config, EditAnywhere, AdvancedDisplay, meta = (ClampMin = "4", UIMin = "4"), Category = Framerate)
	int PhysXTreeRebuildRate;
	/**  If true, the internal physx face to UE face mapping will not be generated. This is a memory optimization available if you do not rely on face indices returned by scene queries. */
	UPROPERTY(config, EditAnywhere, Category = Optimization)
	bool bSuppressFaceRemapTable;
	/** Default gravity. */
	UPROPERTY(config, EditAnywhere, Category = Constants)
	float DefaultGravityZ;
	/** Default terminal velocity for Physics Volumes. */
	UPROPERTY(config, EditAnywhere, Category = Constants)
	float DefaultTerminalVelocity;
	/** Default fluid friction for Physics Volumes. */
	UPROPERTY(config, EditAnywhere, Category = Constants)
	float DefaultFluidFriction;
	/** Amount of memory to reserve for PhysX simulate(), this is per pxscene and will be rounded up to the next 16K boundary */
	UPROPERTY(config, EditAnywhere, Category = Constants, meta = (ClampMin = "0", UIMin = "0"))
	int32 SimulateScratchMemorySize;
	/** Enables persistent contact manifolds. This will generate fewer contact points, but with more accuracy. Reduces stability of stacking, but can help energy conservation.*/
	UPROPERTY(config, EditAnywhere, AdvancedDisplay, Category = Simulation)
	bool bEnablePCM;
	/** Enables stabilization of contacts for slow moving bodies. This will help improve the stability of stacking.*/
	UPROPERTY(config, EditAnywhere, AdvancedDisplay, Category = Simulation)
	bool bEnableStabilization;
	/** Minimum relative velocity required for an object to bounce. A typical value for simulation stability is about 0.2 * gravity*/
	UPROPERTY(config, EditAnywhere, Category = Simulation, meta = (ClampMin = "0", UIMin = "0"))
	float BounceThresholdVelocity;
	/** Friction combine mode, controls how friction is computed for multiple materials. */
	UPROPERTY(config, EditAnywhere, Category = Simulation)
	TEnumAsByte<EFrictionCombineMode::Type> FrictionCombineMode;
	/** Restitution combine mode, controls how restitution is computed for multiple materials. */
	UPROPERTY(config, EditAnywhere, Category = Simulation)
	TEnumAsByte<EFrictionCombineMode::Type> RestitutionCombineMode;
	/** Max angular velocity that a simulated object can achieve.*/
	UPROPERTY(config, EditAnywhere, Category = Simulation)
	float MaxAngularVelocity;
	/** Max velocity which may be used to depenetrate simulated physics objects. 0 means no maximum. */
	UPROPERTY(config, EditAnywhere, Category = Simulation)
	float MaxDepenetrationVelocity;
	/** Contact offset multiplier. When creating a physics shape we look at its bounding volume and multiply its minimum value by this multiplier. A bigger number will generate contact points earlier which results in higher stability at the cost of performance. */
	UPROPERTY(config, EditAnywhere, Category = Simulation, meta = (ClampMin = "0.001", UIMin = "0.001"))
	float ContactOffsetMultiplier;
	/** Min Contact offset. */
	UPROPERTY(config, EditAnywhere, Category = Simulation, meta = (ClampMin = "0.0001", UIMin = "0.0001"))
	float MinContactOffset;
	/** Max Contact offset. */
	UPROPERTY(config, EditAnywhere, Category = Simulation, meta = (ClampMin = "0.001", UIMin = "0.001"))
	float MaxContactOffset;
	FSimulationConfiguration() {
		MaxPhysicsDeltaTime = 0.03333333f;
		bSubstepping = false;
		bSubsteppingAsync = false;
		MaxSubstepDeltaTime = 0.01666666f;
		MaxSubsteps = 6;
		SyncSceneSmoothingFactor = 0.0f;
		InitialAverageFrameRate = 0.01666666f;
		PhysXTreeRebuildRate = 10;
		bSuppressFaceRemapTable = false;
		DefaultGravityZ = -980.f;
		DefaultTerminalVelocity = 4000.f;
		DefaultFluidFriction = 0.3f;
		SimulateScratchMemorySize = 262144;
		bEnablePCM = true;
		bEnableStabilization = false;
		BounceThresholdVelocity = 200.f;
		MaxAngularVelocity = 3600;
		FrictionCombineMode =  EFrictionCombineMode::Average;
		RestitutionCombineMode = EFrictionCombineMode::Average;
		MaxDepenetrationVelocity = 0.f;
		ContactOffsetMultiplier = 0.02f;
		MinContactOffset = 2.f;
		MaxContactOffset = 8.f;
	};
};

USTRUCT(BlueprintType)
struct LIBREFLYER_API FQuadcopterConfiguration {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrottleForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed;
	/*
	Location of camera in relation to quadcopter pivot point
	X = Forward
	Y = Right
	Z = Up
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CameraLocation;

	/** If true Continuous Collision Detection (CCD) will be used for this component */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Collision)
	uint8 bUseCCD : 1;
	/** Override the default max angular velocity */
	UPROPERTY(EditAnywhere, Category = Physics, meta = (editcondition = "bSimulatePhysics", InlineEditConditionToggle))
	uint8 bOverrideMaxAngularVelocity : 1;
	/** The maximum angular velocity for this instance [degrees/s]*/
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Physics, meta = (editcondition = "bOverrideMaxAngularVelocity"))
	float MaxAngularVelocity;
	/** If true, mass will not be automatically computed and you must set it directly */
	UPROPERTY(EditAnywhere, Category = Physics, meta = (InlineEditConditionToggle))
	uint8 bOverrideMass : 1;
	/**Mass of the body in KG. By default we compute this based on physical material and mass scale.
	*@see bOverrideMass to set this directly */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (editcondition = "bOverrideMass", ClampMin = "0.001", UIMin = "0.001", DisplayName = "MassInKg"))
	float MassInKgOverride;
	/** 'Drag' force added to reduce linear movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float LinearDamping;
	/** 'Drag' force added to reduce angular movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float AngularDamping;
	/** User specified offset for the center of mass of this object, from the calculated location */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = Physics, meta = (DisplayName = "Center Of Mass Offset"))
	FVector COMNudge;
	/** Per-instance scaling of mass */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = Physics)
	float MassScale;
	/** Per-instance scaling of inertia (bigger number means  it'll be harder to rotate) */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = Physics)
	FVector InertiaTensorScale;
	/** [PhysX Only] This physics body's solver iteration count for position. Increasing this will be more CPU intensive, but better stabilized.  */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Physics)
	uint8 PositionSolverIterationCount;
	/** [PhysX Only] This physics body's solver iteration count for velocity. Increasing this will be more CPU intensive, but better stabilized. */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Physics)
	uint8 VelocitySolverIterationCount;
	/** Stabilization factor for this body if Physics stabilization is enabled. A higher number will cause more aggressive stabilization at the risk of loss of momentum at low speeds. A value of 0 will disable stabilization for this body.*/
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Physics)
	float StabilizationThresholdMultiplier;
	FQuadcopterConfiguration(){

	};
};

UCLASS(BlueprintType, Config = PlayerPreferences, DefaultConfig)
class LIBREFLYER_API UPlayerPreferences : public UObject {
	GENERATED_BODY()
public:
	// Centemeters per 360 (at 800dpi)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlyingMouseSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FieldOfView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Fisheye;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FisheyeIntensity;
};