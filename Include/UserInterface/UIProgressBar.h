/**
Outlines the basic progress tracking and events functionality required for all types of
progress bars.

@author Nathan */

#ifndef UI_PROGRESS_BAR_H
#define UI_PROGRESS_BAR_H

#include "UIExtendedComponent.h"
#include "UIAudioExtension.h"

class UIProgressBar : 
	public UIExtendedComponent
{
	public:
		enum SoundIds
		{ 
			COMPLETION
		};

		enum Instructions
		{
			SET_TEXTURE,
			SET_SOUND
		};

	private:
		bool m_bPlaySoundOnCompletion;
	
		UIProgressBar();

	protected:
		std::queue<std::pair<Instructions, std::pair<int, std::string>>> m_instructionQueue;
		UIAudioExtension m_audioExtension;
		float m_fProgress;
		bool m_bInverted;

		/**
		Constructs a UIProgressBar with the given name.
		@param sName The name */
		explicit UIProgressBar(std::string sName);

		/**
		Constructs a UIProgressBar with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIProgressBar(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		~UIProgressBar();

		/**
		Called when the progress of the progress bar is updated.
		@param fProgress The new progress value */
		virtual void onProgressUpdated(float fProgress) = 0;

	public:
		/**
		Enables or disables the playing of the completion sound effect when the progress bar enters
		the completed state.
		@param bPlay True to enable the sound effect. */
		void setPlaySoundOnCompletion(bool bPlay);

		/**
		Adds progress to the bar. If the additional progress caused a change in the total progress
		of the bar, an onProgressUpdated event is launched. Should the new total progress reach 
		completion (1), a onProgressCompletion event is launched. Attempting to add progress above 
		1 or below 0 will clamp the target value to these boundaries.
		@param fValue The progress to add. */
		void addProgress(float fValue);

		/**
		Sets the progress of the bar. If the new progress caused a change in the progress of the bar,
		an onProgressUpdated event is launched. Should the new progress reach completion (1), an 
		onProgressCompletion event is launched. Attempting to set progress above 1 or below 0 will
		clamp the target value to these boundaries.
		@param fValue The value to set progress to. */
		void setProgress(float fValue);

		/**
		This function is merely shorthand for a call to addProgress.
		@param fValue The progress to add. 
		@see addProgress */
		void operator+=(float fValue);
		
		/**
		This function is merely shorthand for a call to addProgress with automatic sign inversion.
		(Example: myProgressBar -= 15 is equivalent to myProgressBar += -15).
		@param fValue The progress to minus. 
		@see addProgress */
		void operator-=(float fValue);

		/**
		Returns the current progress. This value is always between 0 and 1. */
		float getProgress() const;

		/**
		Inverts the graphical direction of the progress bar. This function must be overriden to
		correctly invert the component-specific graphics. */
		virtual void setInverted(bool bInverted) = 0;

		void setSoundBuffer(SoundIds id, std::string sResourceKey);
};

#endif