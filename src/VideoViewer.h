#pragma once
#include "Project.h"

#ifndef __AUDACITY_VIDEO_VIEWER__
#define __AUDACITY_VIDEO_VIEWER__

#include "wx/wx.h"
#include <wx/mediactrl.h>

class VideoViewer : public wxFrame
{
public:
	VideoViewer(AudacityProject& project);
	~VideoViewer();

public:
	double audioTime = 0.0;
	bool isMediaLoaded = false;
	int lastMediaPosition = 0;

	AudacityProject& mProject;

	wxMediaCtrl* m_mediactrl;

	void onTimeUpdate(wxIdleEvent& evt);
	void OnMediaLoaded(wxMediaEvent& event);

	void onStateChange(wxCommandEvent& evt);

	
	DECLARE_CLASS(VideoViewer);
	wxDECLARE_EVENT_TABLE();
};

#endif