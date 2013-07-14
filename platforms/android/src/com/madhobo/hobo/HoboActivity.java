/*
 * Copyright (c) 2013, Mathias Hällman. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

package com.madhobo.hobo;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.madhobo.nanaka.*;

public class HoboActivity extends Activity
	implements SurfaceHolder.Callback, View.OnTouchListener
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);
		SurfaceView surfaceView = (SurfaceView)findViewById(R.id.renderview);
		surfaceView.getHolder().addCallback(this);
		surfaceView.setOnTouchListener(this);

		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = getPackageManager();
		try
		{
			appInfo = packMgmr.getApplicationInfo("com.madhobo.hobo", 0);
		}
		catch (NameNotFoundException e)
		{
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;

		Application application = new Application(this);
		Nanaka.Initialize(apkFilePath, application);

		Nanaka.Start();
	}

	@Override
	protected void onPause()
	{
		Nanaka.OnPause();
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		Nanaka.OnResume();
		super.onResume();
	}

	@Override
	protected void onDestroy()
	{
		Nanaka.OnShutdown();
		super.onDestroy();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public boolean onTouch(View view, MotionEvent motionEvent)
	{
		int action = motionEvent.getAction();

		int pointerIdx = 0;

		final int ACTION_POINTER_DOWN = 0;
		final int ACTION_POINTER_UP = 1;
		final int ACTION_MOVE = 2;

		switch (action & MotionEvent.ACTION_MASK)
		{
		case MotionEvent.ACTION_POINTER_DOWN:
			pointerIdx = motionEvent.getActionIndex();
			AddTouchInputEvent(pointerIdx, motionEvent, ACTION_POINTER_DOWN);
			break;

		case MotionEvent.ACTION_DOWN:
			AddTouchInputEvent(pointerIdx, motionEvent, ACTION_POINTER_DOWN);
			break;

		case MotionEvent.ACTION_POINTER_UP:
			pointerIdx = motionEvent.getActionIndex();
			AddTouchInputEvent(pointerIdx, motionEvent, ACTION_POINTER_UP);
			break;

		case MotionEvent.ACTION_UP:
			AddTouchInputEvent(pointerIdx, motionEvent, ACTION_POINTER_UP);
			break;

		case MotionEvent.ACTION_MOVE:
			int pointers = motionEvent.getPointerCount();
			for (int i = 0; i < pointers; i++)
			{
				AddTouchInputEvent(i, motionEvent, ACTION_MOVE);
			}
			break;
		}

		return true;
	}

	private void AddTouchInputEvent(
		int pointerIdx,
		MotionEvent motionEvent,
		int action)
	{
		int pointerId = motionEvent.getPointerId(pointerIdx);
		float x = motionEvent.getX(pointerIdx);
		float y = motionEvent.getY(pointerIdx);

		Nanaka.AddInputEvent(x, y, action, pointerId);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder)
	{
		Nanaka.OnSurfaceCreated(holder.getSurface());
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder)
	{
		Nanaka.OnSurfaceDestroyed();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
	{
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		Nanaka.OnSurfaceChanged(w, h, metrics.density);
	}

	static
	{
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("jniNanaka");
	}
}
