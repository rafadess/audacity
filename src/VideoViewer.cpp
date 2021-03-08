#include "VideoViewer.h"
#include "ProjectAudioIO.h"
#include <AudioIOBase.h>
#include <AudioIO.h>
#include <ViewInfo.h>
#include "Project.h"
#include <ProjectAudioManager.h>

IMPLEMENT_CLASS(VideoViewer, wxFrame);

enum {
	TimeBarFirstID = 2800,
	AudioPositionID,
	MediaCtrlID
};

wxBEGIN_EVENT_TABLE(VideoViewer, wxFrame)
	EVT_IDLE(VideoViewer::onTimeUpdate)
	EVT_MEDIA_LOADED(MediaCtrlID, VideoViewer::OnMediaLoaded)
wxEND_EVENT_TABLE()

VideoViewer::VideoViewer(AudacityProject& project) : wxFrame(nullptr, wxID_ANY, "Video Viewer")
	,mProject{ project }
{	
	wxString path = "D:\\teste.mp4";

	m_mediactrl = new wxMediaCtrl();
	bool wasCreated = m_mediactrl->Create(this, MediaCtrlID, path, wxDefaultPosition, wxDefaultSize, 0L, wxMEDIABACKEND_WMP10);
	if (wasCreated)
	{
		wxLogDebug("Media Control created successfuly");
	}
	else
	{
		wxLogDebug("Failed to create Media Control");
	}

	wxGridSizer* grid = new wxGridSizer(1, 1, 0, 0);

	grid->Add(m_mediactrl);

	wxTheApp->Bind(EVT_AUDIOIO_PLAYBACK, &VideoViewer::onStateChange, this);
	wxTheApp->Bind(EVT_AUDIOIO_CAPTURE, &VideoViewer::onStateChange, this);

	m_mediactrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_DEFAULT);

}

VideoViewer::~VideoViewer()
{
}

void VideoViewer::onTimeUpdate(wxIdleEvent& evt)
{
	evt.Skip();

	auto& projectAudioIO = ProjectAudioIO::Get(mProject);
	if (projectAudioIO.IsAudioActive()) {
		auto gAudioIO = AudioIOBase::Get();
		audioTime = gAudioIO->GetStreamTime();
	}
	else {
		const auto& playRegion = ViewInfo::Get(mProject).playRegion;
		audioTime = playRegion.GetStart();
	}
	
	double position = (int)(wxMax(0.0, audioTime) * 1000);
	
	if (isMediaLoaded && lastMediaPosition != position)
	{
		
		wxFileOffset mediaPosition = m_mediactrl->Length();
		//m_mediactrl->Seek(61000);
		//wxLogDebug("%i", (int)m_mediactrl->Length());
	}
}

void VideoViewer::OnMediaLoaded(wxMediaEvent& evt)
{
	evt.Skip();
	wxLogDebug("Media Loaded!");
	isMediaLoaded = true;
}

void VideoViewer::onStateChange(wxCommandEvent& evt)
{
	evt.Skip();

	wxLogDebug("State changed");
	if (!isMediaLoaded) return;

	auto gAudioIO = AudioIOBase::Get();
	auto& projectAudioManager = ProjectAudioManager::Get(mProject);

	//bool isPlaying = projectAudioManager.Playing();
	if (projectAudioManager.Playing() || projectAudioManager.Recording())
	{
		m_mediactrl->Play();
	}
	else 
	{
		//m_mediactrl->Pause();
		//int mediaPosition = (int)((wxMax(0.0, audioTime) * 1000) + 400);
		//m_mediactrl->Seek(mediaPosition);
		//wxLogDebug("%i", mediaPosition);
	}
}


