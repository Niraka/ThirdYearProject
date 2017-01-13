/**
@author Nathan
*/

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
	private:
		bool m_bPlaying;
		bool m_bLoop;
		long m_iDuration;

	protected: 

	public:
		Animation(); 
		
		void play();
		void stop();
		void setLooping(bool bLooping);
		void setDuration(long iMillis);

		bool isPlaying();
};

#endif