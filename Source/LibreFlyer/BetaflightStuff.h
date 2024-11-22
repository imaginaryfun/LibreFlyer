#pragma once
//#include <stdint.h>
//#include <math.h>
#include "BetaflightStuff.generated.h"

#define CONTROL_RATE_CONFIG_RATE_LIMIT_MIN  200
#define CONTROL_RATE_CONFIG_RATE_LIMIT_MAX  1998
#define RC_RATE_INCREMENTAL 14.54f
#define MAX_RATE_PROFILE_NAME_LENGTH 8u
#define SETPOINT_RATE_LIMIT_MIN -1998.0f
#define SETPOINT_RATE_LIMIT_MAX 1998.0f

UENUM(BlueprintType)
enum class ERatesType : uint8 {
	Betaflight,
	Raceflight,
	KISS,
	ActualRates,
	QuickRates
};

USTRUCT(BlueprintType)
struct FBetaflightRates {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERatesType RatesType;
	/*
		0 = Roll
		1 = Pitch
		2 = Yaw
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> RcRates;
	/*
		0 = Roll
		1 = Pitch
		2 = Yaw
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> SuperRates;
	/*
		0 = Roll
		1 = Pitch
		2 = Yaw
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> RcExpo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 QuickRatesRcExpo;

	FBetaflightRates() {
		RatesType = ERatesType::Betaflight;
		RcRates.Add(100);
		RcRates.Add(100);
		RcRates.Add(100);
		RcExpo.Add(0);
		RcExpo.Add(0);
		RcExpo.Add(0);
		SuperRates.Add(70);
		SuperRates.Add(70);
		SuperRates.Add(70);
		QuickRatesRcExpo = 0;
	};
	static float Clamp(const float X, const float Min, const float Max) {
		return X < Min ? Min : X < Max ? X : Max;
	};
	void Apply(int axis, float& rcCommandf, const float& rcCommandfAbs) {
		if (RatesType == ERatesType::Betaflight) {
			if (RcExpo[axis]) {
				const float expof = RcExpo[axis] / 100.0f;
				rcCommandf = rcCommandf * powf(rcCommandfAbs, 3.f) * expof + rcCommandf * (1 - expof);
			}
			float rcRate = RcRates[axis] / 100.0f;
			if (rcRate > 2.0f) {
				rcRate += RC_RATE_INCREMENTAL * (rcRate - 2.0f);
			}
			float angleRate = 200.0f * rcRate * rcCommandf;
			if (SuperRates[axis]) {
				const float rcSuperfactor = 1.0f / (Clamp(1.0f - (rcCommandfAbs * (SuperRates[axis] / 100.0f)), 0.01f, 1.00f));
				angleRate *= rcSuperfactor;
			}
			rcCommandf = angleRate;
		}
		else if (RatesType == ERatesType::Raceflight) {
			rcCommandf = ((1.0f + 0.01f * RcExpo[axis] * (rcCommandf * rcCommandf - 1.0f)) * rcCommandf);
			float angleRate = 10.0f * RcRates[axis] * rcCommandf;
			angleRate = angleRate * (1 + rcCommandfAbs * (float)SuperRates[axis] * 0.01f);
			rcCommandf = angleRate;
		}
		else if (RatesType == ERatesType::KISS) {
			const float rcCurvef = RcExpo[axis] / 100.0f;
			float kissRpyUseRates = 1.0f / (Clamp(1.0f - (rcCommandfAbs * (SuperRates[axis] / 100.0f)), 0.01f, 1.00f));
			float kissRcCommandf = (powf(rcCommandfAbs, 3.f) * rcCurvef + rcCommandf * (1 - rcCurvef)) * (RcRates[axis] / 1000.0f);
			float kissAngle = Clamp(((2000.0f * kissRpyUseRates) * kissRcCommandf), SETPOINT_RATE_LIMIT_MIN, SETPOINT_RATE_LIMIT_MAX);
			rcCommandf = kissAngle;
		}
		else if (RatesType == ERatesType::ActualRates) {
			float expof = RcExpo[axis] / 100.0f;
			expof = rcCommandfAbs * (powf(rcCommandfAbs, 5.f) * expof + rcCommandf * (1 - expof));
			const float centerSensitivity = RcRates[axis] * 10.0f;
			const float stickMovement = FMath::Max(0.f, SuperRates[axis] * 10.0f - centerSensitivity);
			const float angleRate = rcCommandf * centerSensitivity + stickMovement * expof;
			rcCommandf = angleRate;
		}
		else if (RatesType == ERatesType::QuickRates) {
			const uint16 rcRate = RcRates[axis] * 2;
			const uint16 maxDPS = FMath::Max<uint16>(SuperRates[axis] * 10, rcRate);
			const float expof = RcExpo[axis] / 100.0f;
			const float superFactorConfig = ((float)maxDPS / rcRate - 1) / ((float)maxDPS / rcRate);

			float curve;
			float superFactor;
			float angleRate;
			if (QuickRatesRcExpo) {
				curve = powf(rcCommandfAbs, 3.f) * expof + rcCommandf * (1 - expof);
				superFactor = 1.0f / (Clamp(1.0f - (rcCommandfAbs * superFactorConfig), 0.01f, 1.00f));
				angleRate = Clamp(curve * rcRate * superFactor, SETPOINT_RATE_LIMIT_MIN, SETPOINT_RATE_LIMIT_MAX);
			}
			else {
				curve = powf(rcCommandfAbs, 3.f) * expof + rcCommandfAbs * (1 - expof);
				superFactor = 1.0f / (Clamp(1.0f - (curve * superFactorConfig), 0.01f, 1.00f));
				angleRate = Clamp(rcCommandf * rcRate * superFactor, SETPOINT_RATE_LIMIT_MIN, SETPOINT_RATE_LIMIT_MAX);
			}
			rcCommandf = angleRate;
		}
	};
};
