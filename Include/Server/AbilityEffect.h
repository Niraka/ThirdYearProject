#ifndef ABILITY_EFFECT_H
#define ABILITY_EFFECT_H

class AbilityEffect
{
public:
	enum class Types
	{
		DAMAGE_BUFF,      // Affects unit status
		DAMAGE,           // Applies damage instantly
		DAMAGE_OVER_TIME  // Applies damage over time
	};

	private:
		Types m_type;              // The type of effect
		int m_iDurationMillis;     // The duration of the effect
		int m_iContextValue;       // The value to apply to the

	protected:

	public:
		AbilityEffect();

		/**
		Constructs an AbilityEffect. 
		@param type The type of effect
		@param iDuration The duration, in milliseconds */
		AbilityEffect(Types type, int iDuration);

		/**
		Constructs an AbilityEffect.
		@param type The type of effect
		@param iDuration The duration, in milliseconds
		@param iContext The context value */
		AbilityEffect(Types type, int iDuration, int iContext);

		/**
		Sets the duration in milliseconds. Use -1 for infinite duration or 0 for
		instant.
		@param iMillis The duration */
		void setDuration(int iMillis);

		/**
		Sets the context value. The context value depends on the type of effect.
		@param iContextValue The context value*/
		void setContextValue(int iContextValue);

		/**
		Returns the type of effect. 
		@return The type of effect */
		const Types getType() const;

		/**
		Returns the duration of the effect in milliseconds. 0 indicates an instant
		effect whereas -1 indicates an infinite duration.
		@return The duration, in milliseconds */
		const int getDuration() const;

		/**
		Returns the context value. The context value returns an integer whose 
		interpretation depends on the type of effect. 
		@return The context value */
		const int getContextValue() const;
};

#endif