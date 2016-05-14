#include "Animation.h"

namespace XUI
{

	std::shared_ptr<AnimationInterpolator> Animation::mDefaultInterpolator = LinearInterpolator::linearInterpolator();

	Animation::~Animation() {

	}

	bool Animation::setProcessFun(const std::function<void(double)> &fun) {
		mProcessFun = fun;
		return true;
	}
	void Animation::setAnimationInterpolator(std::shared_ptr<AnimationInterpolator> interpolator) {
		mInterpolator = interpolator;
	}
	void Animation::setDurationMS(unsigned long ms) {
		if (ms > 0)
		{
			mDurationMS = ms;
		}
	}
	void Animation::setRepeatTimes(long times) {
		mRepeatTimes = times;
	}
	bool Animation::play() {
		if (mState != AnimatingStates::Playing && mProcessFun)
		{
			mProcessedMs = 0;
			mRepeatedTimes = 0;
			mState = AnimatingStates::Playing;
			return true;
		}
		return false;
	}
	void Animation::pause() {
		if (mState == AnimatingStates::Playing)
		{
			mState = AnimatingStates::Paused;
		}
	}
	void Animation::stop() {
		if (mState != AnimatingStates::Stopped)
		{
			mState = AnimatingStates::Stopped;
			mProcessedMs = 0;
		}
	}
	void Animation::process(unsigned long ms) {
		mProcessedMs += ms;
		if (mProcessedMs >= mDurationMS)
		{
			mProcessFun(1);
			++mRepeatedTimes;
			if (mRepeatedTimes > mRepeatTimes)
			{
				stop();
			}
			else {
				mProcessedMs = 0;
			}
		}
		else {
			mProcessFun(mInterpolator->calculate(mDurationMS, mProcessedMs));
		}
	}
	AnimatingStates Animation::state() {
		return mState;
	}

}