#include "Animation.h"

namespace XUI
{

	std::shared_ptr<AnimationInterpolator> Animation::mDefaultInterpolator = LinearInterpolator::linearInterpolator();

	Animation::~Animation() {

	}

	bool Animation::setProcessFun(const std::function<void(Animation*, unsigned long ms)> &fun) {
		mProcessFun = fun;
		return true;
	}
	
	Animation& Animation::setDurationMS(unsigned long ms) {
		if (ms > 0)
		{
			mDurationMS = ms;
		}
        return *this;
	}
	Animation& Animation::setRepeatTimes(long times) {
		mRepeatTimes = times;
        return *this;
	}
	bool Animation::play() {
		if (mState != AnimatingStates::Playing && mProcessFun)
		{
			mProcessedMs = 0;
			mRepeatedTimes = 0;
			mState = AnimatingStates::Playing;
			if (onStart) {
				onStart();
			}
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
			if(onStop) {
				onStop();
			}
		}
	}
	void Animation::process(unsigned long ms) {
		mProcessedMs += ms;
		if (mProcessedMs >= mDurationMS)
		{
            mProcessedMs =  mDurationMS;
			mProcessFun(this,ms - mProcessedMs + mDurationMS);
			++mRepeatedTimes;
			if (mRepeatedTimes > mRepeatTimes)
			{
				stop();
			}
			else {
				mProcessedMs = 0;
			}
			if (onFinish) {
				onFinish();
			}
		}
		else {
			mProcessFun(this, ms);
		}
	}
	AnimatingStates Animation::state() {
		return mState;
	}
    

}