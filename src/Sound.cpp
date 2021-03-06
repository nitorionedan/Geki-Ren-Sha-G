#include "Sound.hpp"
#include "Graphics2D.hpp"
#include "Graphic.hpp"
#include <DxLib.h>
#include <cassert>
#include <cstdlib>

constexpr double SoundFadeoutTime = 180.;


bool Sound::isLoaded = false;
Graphic Sound::gr;
std::vector<Sound::SoundFile*> Sound::mSound;


void Sound::Update()
{
	Update_fadeout();

#ifdef _DEBUG
	/* check num of playing sound mem*/
	int c_play = 0;
	for (auto i : mSound)
	{
		if (CheckSoundMem(i->handle) == TRUE)
			++c_play;
	}
#endif
}


void Sound::Update_fadeout()
{
	for (auto sound : mSound)
	{
		if (CheckSoundMem(sound->handle) == TRUE && sound->isFadeOut)
		{
			sound->volume -= sound->Max_vol / SoundFadeoutTime;
			if (sound->volume <= 0.)
			{
				StopSoundMem(sound->handle);
				sound->SetVolume(sound->Max_vol);
				sound->isFadeOut = false;
			}
			sound->SetVolume(sound->volume);
			break;
		}
	}
}


void Sound::Load()
{
	if(isLoaded)
	{
		assert(!"You are about to load again");
		return;
	}

	int tmpGr = LoadGraph("GRAPH/MENU/title.png");
	int tmpGrS[32];
	int tmpGrB[32];
	int tmpGrP[32];
	LoadDivGraph("GRAPH/GAME/itemS.png", _countof(tmpGrS), 8, 4, 16, 14, tmpGrS);
	LoadDivGraph("GRAPH/GAME/itemB.png", _countof(tmpGrB), 8, 4, 16, 14, tmpGrB);
	LoadDivGraph("GRAPH/GAME/itemP.png", _countof(tmpGrP), 8, 4, 16, 14, tmpGrP);

	SetUseASyncLoadFlag(TRUE);

	isLoaded = true;
	
	mSound.reserve(static_cast<int>(eSound::eSoundNum));

	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/opening.mp3"), 170)); // opening
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/s1.mp3"), 170)); // 1
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/s2.mp3"), 170)); // 2
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // 3
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // 4
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // 5
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // 6
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // 0
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossA.mp3"), 170)); // A
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/bossB.mp3"), 170)); // B
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // C
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // D
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // E
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // F
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // G
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // H
													   
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // gameover
	mSound.push_back(new SoundFile(LoadSoundMem("SOUND/SE/1up.mp3"), 170)); // nameentry

	assert(mSound.size() == static_cast<int>(eSound::eSoundNum) && "num of sound file not goes well with mSound.size()");

	bool finishFlag = false;
	int c_load = 0;
	int FrameStartTime = GetNowCount();																// 開始時間を設定
	while (1)
	{
		if (ProcessMessage() != 0)
			exit(-1);

		while (GetNowCount() - FrameStartTime < 1000 / 60) {}									// 1/60 秒まで待つ
		FrameStartTime = GetNowCount();															// 現在のカウントを保存
		
		++c_load;

		int i = 0;
		for (auto sound : mSound)
		{
			// finish
			if (CheckHandleASyncLoad(sound->handle) == FALSE)
				++i;
			else
			{
				int x = CheckHandleASyncLoad(sound->handle);
				if (x == -1)
				{
					MessageBox(NULL, "もう一度起動してください。", "サウンド読み込みエラー（ヒープ領域が足りないorz）", MB_OK);
					exit(-1);
				}
			}

			if(i == mSound.size())
				finishFlag = true;
		}

		ClearDrawScreen();

		DrawRotaGraph(320, 240, 1., 0., tmpGr, TRUE);
		DrawAnime(320, 320, 2., 0., c_load,     _countof(tmpGrS), 4, tmpGrS);
		DrawAnime(220, 320, 2., 0., c_load + 2, _countof(tmpGrB), 4, tmpGrB);
		DrawAnime(420, 320, 2., 0., c_load + 4, _countof(tmpGrP), 4, tmpGrP);
		DrawFormatString(0, 30, GetColor(0, 255, 0), "読み込み率%10.0lf％", ( (double)i / (double)mSound.size() ) * 100);
		DrawBox(0, 0, i * 10, 30, GetColor(0, 255, 0), TRUE);
		//gr.DrawMyString2(0, 0, "NOW LOADING...", 16, true, 2.);

		if (finishFlag)
			break;

		Sleep(1);
	}

	SetUseASyncLoadFlag(FALSE);

	DeleteGraph(tmpGr);
	for (auto i : tmpGrS)
		DeleteGraph(i);
	for (auto i : tmpGrP)
		DeleteGraph(i);
	for (auto i : tmpGrB)
		DeleteGraph(i);
}


void Sound::Delete()
{
	for (auto i : mSound)
		DeleteSoundMem(i->handle);
	for (auto i : mSound)
		delete i;
	mSound.clear();
	isLoaded = false;
}


void Sound::Play(eSound sound)
{
	PlaySoundMem(mSound[static_cast<int>(sound)]->handle, DX_PLAYTYPE_LOOP);

	int volume = 0;

	switch (sound)
	{
	case eSound::opening:    volume = 170; break;
	case eSound::stage1:     volume = 170; break;
	case eSound::stage2:     volume = 170; break;
	case eSound::stage3:     volume = 170; break;
	case eSound::stage4:     volume = 170; break;
	case eSound::stage5:     volume = 170; break;
	case eSound::stage6:     volume = 170; break;
	case eSound::stage0:     volume = 170; break;
	case eSound::bossA:      volume = 170; break;
	case eSound::bossB:	     volume = 170; break;
	case eSound::bossC:	     volume = 170; break;
	case eSound::bossD:	     volume = 170; break;
	case eSound::bossE:	     volume = 170; break;
	case eSound::bossF:	     volume = 170; break;
	case eSound::bossG:	     volume = 170; break;
	case eSound::bossH:	     volume = 170; break;
	case eSound::gameover:   volume = 170; break;
	case eSound::name_entry: volume = 170; break;
	case eSound::eSoundNum:	 volume = 170; break;
	default: assert(!"error");
	}

	::ChangeVolumeSoundMem(volume, mSound[static_cast<int>(sound)]->handle);
}


void Sound::Stop()
{
	for (auto sound : mSound)
	{
		if (CheckSoundMem(sound->handle) == TRUE)
		{
			StopSoundMem(sound->handle);
			break;
		}
	}
}


void Sound::FadeOutStop()
{
	for (auto sound : mSound)
	{
		if (CheckSoundMem(sound->handle) == TRUE && sound->isFadeOut == false)
		{
			sound->isFadeOut = true;
			break;
		}
	}
}


void Sound::SetVolume(eSound sound, int volume)
{
	mSound[static_cast<int>(sound)]->volume = volume;
	ChangeVolumeSoundMem(volume, mSound[static_cast<int>(sound)]->handle);
}


bool Sound::IsPlaySome()
{
	for (auto i : mSound)
	{
		if (CheckSoundMem(i->handle) == TRUE)
			return true;
	}

	return false;
}


Sound::SoundFile::SoundFile(int handle, double Max_vol)
	: Max_vol(Max_vol)
{
	this->handle = handle;
	volume = Max_vol;
	isFadeOut = false;
}


void Sound::SoundFile::SetVolume(int volumePal)
{
	volume = volumePal;
	::ChangeVolumeSoundMem(volume, handle);
}
