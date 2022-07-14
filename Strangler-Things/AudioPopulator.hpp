#pragma once

#include "AudioSource.hpp"
#include "SfxrClip.hpp"

#include "AudioPopulatorBase.hpp"
#include "GeneratorComponent.hpp"
#include "OnLateGameplayUpdateComponent.hpp"
#include "PlayerComponent.hpp"


class AudioPopulator : public AudioPopulatorBase
{
public:
	void Global() override { };
	void Scene( GameObject a_Object ) override { };
	void HorizontalExit( GameObject a_Object ) override { };
	void Player( GameObject a_Object ) override
	{
		auto* Player = a_Object.GetComponent< PlayerComponent >();

		auto footstepsSourceObj = GameObject::Instantiate(a_Object);
		{
			auto* audioSource = footstepsSourceObj.AddComponent< AudioSource >();
			auto footstepSfx = Resource::Load< SfxrClip >("footstep"_H);
			audioSource->LoadSfx(footstepSfx);
			audioSource->SetRolloffFactor(0.1f);
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
						audioSource->SetLooping(true);
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
	void TentacleStart( GameObject a_Object ) override { };
	void Random( GameObject a_Object ) override { };
	void Fuel( GameObject a_Object ) override { };
	void Generator( GameObject a_Object ) override
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
	void Bonus( GameObject a_Object ) override { };

};

static AudioPopulator AP_;