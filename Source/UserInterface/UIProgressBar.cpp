#include "UIProgressBar.h"

UIProgressBar::UIProgressBar() : 
UIExtendedComponent("NEVER_CALLED"),
m_audioExtension(this)
{
	m_bPlaySoundOnCompletion = false;
	m_bInverted = false;
	setProgress(0.f);
}

UIProgressBar::UIProgressBar(std::string sName) : 
UIExtendedComponent(sName),
m_audioExtension(this)
{
	m_bPlaySoundOnCompletion = false;
	m_bInverted = false;
	setProgress(0.f);
}

UIProgressBar::UIProgressBar(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIExtendedComponent(sName, xOffset, yOffset, width, height),
m_audioExtension(this)
{
	m_bPlaySoundOnCompletion = false;
	m_bInverted = false;
	setProgress(0.f);
}

UIProgressBar::~UIProgressBar()
{
}

void UIProgressBar::setPlaySoundOnCompletion(bool bPlay)
{
	m_bPlaySoundOnCompletion = bPlay;
}

void UIProgressBar::setProgress(float fValue)
{
	// Detect change
	bool launchEvent = false;	
	if (fValue != m_fProgress)
	{
		launchEvent = true;
	}

	// Set value (cap if necessary)
	float oldProgress = m_fProgress;
	if (fValue < 0.f)
	{
		m_fProgress = 0.f;
	}
	else if (fValue > 1.0)
	{
		m_fProgress = 1.0;
	}
	else
	{
		m_fProgress = fValue;
	}

	// Launch event if necessary
	if (launchEvent)
	{
		onProgressUpdated(m_fProgress);
		m_eventQueue.push(EventInfo(getName(), EventTypes::PROGRESS_UPDATED));
	}
	if (oldProgress != 1.0 && m_fProgress == 1.0)
	{
		if (m_bPlaySoundOnCompletion)
		{
			m_audioExtension.playSound(SoundIds::COMPLETION);
		}
		m_eventQueue.push(EventInfo(getName(), EventTypes::PROGRESS_COMPLETED));
	}
}

void UIProgressBar::addProgress(float fValue)
{
	// We cannot increase above 1 or decrease below 0 - discard operation
	if ((m_fProgress == 1.0 && fValue >= 0.f) ||
		(m_fProgress == 0.f && fValue <= 0.f))
	{
		return;
	}
	
	// Cap at 0 or 1
	float result = m_fProgress + fValue;
	if (result < 0.f)
	{
		result = 0.f;
	}
	else if (result > 1.0)
	{
		result = 1.0;
	}

	// Decide which events to launch and set new progress value			
	if (result != m_fProgress)
	{
		m_fProgress = result;			
		onProgressUpdated(m_fProgress);
		m_eventQueue.push(EventInfo(getName(), EventTypes::PROGRESS_UPDATED));
	}
	if (m_fProgress == 1.0)
	{
		if (m_bPlaySoundOnCompletion)
		{
			m_audioExtension.playSound(SoundIds::COMPLETION);
		}
		m_eventQueue.push(EventInfo(getName(), EventTypes::PROGRESS_COMPLETED));
	}
}

void UIProgressBar::operator+=(float fValue)
{
	addProgress(fValue);
}

void UIProgressBar::operator-=(float fValue)
{
	addProgress(-fValue);
}

float UIProgressBar::getProgress() const
{
	return m_fProgress;
}

void UIProgressBar::setSoundBuffer(SoundIds id, std::string sResourceKey)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_SOUND, std::make_pair(id, sResourceKey)));
	m_lock.unlock();
}