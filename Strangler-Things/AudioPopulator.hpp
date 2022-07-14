#pragma once

#include "AudioClip.hpp"
#include "AudioSource.hpp"
#include "SfxrClip.hpp"

#include "GeneratorComponent.hpp"
#include "OnLateGameplayUpdateComponent.hpp"
#include "PlayerComponent.hpp"


class AudioPopulator
{
public:
	void Global() {
		auto music = Resource::Load< AudioClip >("music"_H);
		auto musicObject = GameObject::Instantiate(Name("music"));
		auto* musicSource = musicObject.AddComponent<AudioSource>();
		musicSource->Set3d(false);
		musicSource->SetVolume(0.5f);
		musicSource->LoadWav(music);
		musicSource->Play();
		musicSource->SetLooping(true);
		musicSource->SetLoopPoint(9.975);
	};
	void Scene( GameObject a_Object, Hash a_LevelName ) { };
	void HorizontalExit( GameObject a_Object ) { };
	void Player( GameObject a_Object )
	{
		auto* Player = a_Object.GetComponent< PlayerComponent >();

		auto footstepsSourceObj = GameObject::Instantiate(a_Object);
		{
			auto* audioSource = footstepsSourceObj.AddComponent< AudioSource >();
			auto footstepSfx = Resource::Load< SfxrClip >("footstep"_H);
			audioSource->LoadSfx(footstepSfx);
			audioSource->SetRolloffFactor(0.1f);
			audioSource->SetLooping(true);
		}

		footstepsSourceObj.AddComponent< OnLateGameplayUpdateComponent >()
			->Init([=]()
			{
				auto* nonConstPlayer = ((GameObject)a_Object).GetComponent< PlayerComponent >();
				auto* audioSource = ((GameObject)footstepsSourceObj).GetComponent< AudioSource >();
				
				if (nonConstPlayer->GetLastMovementOffset() != Vector3::Zero)
				{
					if (audioSource && !audioSource->IsPlaying())
					{
						audioSource->Play();
					}
				}
				else
				{
					if (audioSource && audioSource->IsPlaying())
					{
						audioSource->Stop();
					}
				}
			});

		auto pickUpSourceObj = GameObject::Instantiate(a_Object);
		pickUpSourceObj.AddComponent< AudioSource >();

		Player->OnPickedUpObject += [=]()
		{
			auto* Audio = ((GameObject)pickUpSourceObj).GetComponent< AudioSource >();
			auto PUSound = Resource::Load< SfxrClip >("pickup"_H);
			Audio->LoadSfx(PUSound);
			Audio->SetRolloffFactor(0.1f);
			Audio->Play();
		};

		Player->OnDroppedObject += [=]()
		{
			auto* Audio = ((GameObject)pickUpSourceObj).GetComponent< AudioSource >();
			auto PUSound = Resource::Load< SfxrClip >("drop"_H);
			Audio->LoadSfx(PUSound);
			Audio->SetRolloffFactor(0.1f);
			Audio->Play();
		};
	};
	void TentacleStart( GameObject a_Object ) { };
	void Random( GameObject a_Object ) { };
	void Fuel( GameObject a_Object ) { };
	void Generator( GameObject a_Object )
	{
		auto audioSourceObj = GameObject::Instantiate( a_Object );
		auto* audioSource = audioSourceObj.AddComponent< AudioSource >();
		auto sound = Resource::Load< SfxrClip >("powerup"_H);
		audioSource->LoadSfx(sound);
		audioSource->SetRolloffFactor(0.1f);

		a_Object.GetComponent< GeneratorComponent >()
			->OnConsumedFuel += [=]()
			{
				((GameObject)audioSourceObj).GetComponent< AudioSource >()->Play();
			};
	};
	void Bonus( GameObject a_Object ) { };

};

static AudioPopulator AP_;