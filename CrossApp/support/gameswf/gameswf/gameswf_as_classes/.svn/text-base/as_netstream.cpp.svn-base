// as_netstream.cpp	-- Vitaly Alexeev <tishka92@yahoo.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gameswf/gameswf_as_classes/as_netstream.h"
#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_root.h"
#include "base/tu_timer.h"

#if TU_CONFIG_LINK_TO_FFMPEG == 1

namespace gameswf
{

	// it is running in decoder thread
	static void netstream_server(void* arg)
	{
		as_netstream* ns = (as_netstream*) arg;
		ns->run();
	}

	// audio callback is running in sound handler thread
	static void audio_streamer(as_object* netstream, Uint8* stream, int len)
	{
		as_netstream* ns = cast_to<as_netstream>(netstream);
		assert(ns);
		ns->audio_callback(stream, len);
	}

	static hash<int, tu_string> s_netstream_event_level;
	static hash<int, tu_string> s_netstream_event_code;

	as_netstream::as_netstream(player* player):
		as_object(player),
		m_FormatCtx(NULL),
		m_VCodecCtx(NULL),
		m_video_stream(NULL),
		m_ACodecCtx(NULL),
		m_audio_stream(NULL),
		m_start_time(0.0f),
		m_video_time(0.0f),
		m_video_index(-1),              
		m_audio_index(-1),
		m_video_data(NULL),
		m_convert_ctx(NULL),
		m_status(STOP),
		m_seek_time(-1.0f),
		m_buffer_time(0.1f)	// The default value is 0.1 second
	{
		// fill static hash once
		if (s_netstream_event_level.size() == 0)
		{
			s_netstream_event_level.add(status, "status");
			s_netstream_event_level.add(error, "error");
		}

		if (s_netstream_event_code.size() == 0)
		{
			s_netstream_event_code.add(playStreamNotFound, "NetStream.Play.StreamNotFound");
			s_netstream_event_code.add(playStart, "NetStream.Play.Start");
			s_netstream_event_code.add(playStop, "NetStream.Play.Stop");
			s_netstream_event_code.add(seekNotify, "NetStream.Seek.Notify");
			s_netstream_event_code.add(seekInvalidTime, "NetStream.Seek.InvalidTime");
			s_netstream_event_code.add(bufferEmpty, "NetStream.Buffer.Empty");
			s_netstream_event_code.add(bufferFull, "NetStream.Buffer.Full");
		}

		av_register_all();
	}

	as_netstream::~as_netstream()
	{
		switch (m_status)
		{
			default:
				break;
			case PLAY:
				m_status = STOP;
				m_thread->wait();
				break;
			case PAUSE:
				m_status = STOP;
				m_decoder.signal();
				m_thread->wait();
				break;
		}
	}

	void as_netstream::play(const char* url)
	{
		switch (m_status)
		{
			default:
				break;
			case PAUSE:
				m_status = PLAY;
				m_decoder.signal();
				break;
			case STOP:
			{
				// is path relative ?
				tu_string infile = get_player()->get_workdir();
				if (strstr(url, ":") || url[0] == '/')
				{
					infile = "";
				}
				infile += url;
				m_url = infile;

				get_root()->add_listener(this);
				if (open_stream(m_url.c_str()) == true)
				{
					sound_handler* sound = get_sound_handler();
					if (sound)
					{
						sound->attach_aux_streamer(audio_streamer, this);
					}

					m_thread = new tu_thread(netstream_server, this);
				}
				break;
			}
		}
	}

	void as_netstream::close()
	{
		switch (m_status)
		{
			default:
				break;
			case PLAY:
				m_status = STOP;
				break;
			case PAUSE:
				m_status = STOP;
				m_decoder.signal();
				break;
		}
	}

	// seek_time in sec
	void as_netstream::seek(double seek_time)
	{
		switch (m_status)
		{
			default:
				break;

			case PLAY:
				if (seek_time < 0)
				{
					seek_time = 0;
				}
				m_seek_time = seek_time;
				break;

			case PAUSE:
				if (seek_time < 0)
				{
					seek_time = 0;
				}
				m_seek_time = seek_time;

				m_status = PLAY;
				m_decoder.signal();
				break;
		}
	}

	// return current time in sec
	double as_netstream::now() const
	{
		return (double) tu_timer::get_ticks() / 1000.0;
	}

	// it is running in decoder thread
	void as_netstream::run()
	{
		set_status(status, playStart);

		m_start_time = now();
		m_video_time = 0;
		
		m_status = PLAY;
		while (m_status == PLAY || m_status == PAUSE)
		{
			if (m_status == PAUSE)
			{
				double paused = now();
				m_decoder.wait();
				m_start_time += now() - paused;
				continue;
			}

			// seek request
			if (m_seek_time >= 0)
			{
				int64 timestamp = (int64) (m_seek_time * AV_TIME_BASE);
				int flags = m_seek_time > m_video_time ? 0 : AVSEEK_FLAG_BACKWARD;
				int ret = av_seek_frame(m_FormatCtx, -1, timestamp, flags);
				if (ret == 0)
				{
					m_aq.clear();
					m_vq.clear();
					m_start_time += m_video_time - m_seek_time;
					m_video_time = m_seek_time;
					set_status(status, seekNotify);
				}
				else
				{
					set_status(error, seekInvalidTime);
				}
				m_seek_time = -1;
			}

			if (get_bufferlength() < m_buffer_time)
			{
				//printf("m_buffer_length=%f, queue_size=%d\n", get_bufferlength(), m_vq.size()); 

				AVPacket pkt;
				int rc = av_read_frame(m_FormatCtx, &pkt);
				if (rc < 0)
				{
					if (m_vq.size() == 0)
					{
						break;
					}
				}
				else
				{
					if (pkt.stream_index == m_video_index)
					{
						m_vq.push(new av_packet(pkt));
					}
					else
					if (pkt.stream_index == m_audio_index)
					{
						if (get_sound_handler())
						{
							m_aq.push(new av_packet(pkt));
						}
					}
					else
					{
						continue;
					}
				}
			}

			// skip expired video frames
			double current_time = now() - m_start_time;
			while (current_time >= m_video_time)
			{
				gc_ptr<av_packet> packet;
				if (m_vq.pop(&packet))
				{
					const AVPacket& pkt = packet->get_packet();

					// update video clock with pts, if present
					if (pkt.dts > 0)
					{
						m_video_time = av_q2d(m_video_stream->time_base) * pkt.dts;
					}
					m_video_time += av_q2d(m_video_stream->codec->time_base);	// +frame_delay

					set_video_data(decode_video(pkt));
				}
				else
				{
					// no packets in queue
//					set_status("status", "NetStream.Buffer.Empty");
					break;
				}
			}

			// Don't hog the CPU.
			// Queues have filled, video frame have shown
			// now it is possible and to have a rest

			int delay = (int) (1000 * (m_video_time - current_time));
			
			// hack, adjust m_start_time after seek
			if (delay > 50)
			{
				m_start_time -= (m_video_time - current_time);
				current_time = now() - m_start_time;
				delay = (int) (1000 * (m_video_time - current_time));
			}

			assert(delay <= 50);

			if (delay > 0)
			{
				if (get_bufferlength() >= m_buffer_time)
				{
//					set_status("status", "NetStream.Buffer.Full");
					tu_timer::sleep(delay);
				}
//				printf("current_time=%f, video_time=%f, delay=%d\n", current_time, m_video_time, delay);
			}
		}

		sound_handler* sound = get_sound_handler();
		if (sound)
		{
			sound->detach_aux_streamer(this);
		}

		close_stream();

		set_status(status, playStop);
		m_status = STOP;
	}

	// it is running in sound mixer thread
	void as_netstream::audio_callback(Uint8* stream, int len)
	{
		while (len > 0 && m_status == PLAY)
		{
			// decode sound
			if (m_sound == NULL)
			{
				gc_ptr<av_packet> audio;
				if (m_aq.pop(&audio))
				{
					Sint16* sample;
					int size;
					decode_audio(audio->get_packet(), &sample, &size);
					m_sound = new decoded_sound(sample, size);
					continue;
				}
				break;
			}
			else
			{
				int n = m_sound->extract(stream, len);
				stream += n;
				len -= n;
				if (m_sound->size() == 0)
				{
					m_sound = NULL;
				}
			}
		}

	}

	void as_netstream::advance(float delta_time)
	{
		stream_event ev;
		while(m_event.pop(&ev))
		{
//			printf("pop status: %s %s\n", ev.level.c_str(), ev.code.c_str());		

			// keep this alive during execution!
			gc_ptr<as_object>	this_ptr(this);

			as_value function;
			if (get_member("onStatus", &function))
			{
				gc_ptr<as_object> infoObject = new as_object(get_player());
				infoObject->set_member("level", s_netstream_event_level[ev.level].c_str());
				infoObject->set_member("code", s_netstream_event_code[ev.code].c_str());

				as_environment env(get_player());
				env.push(infoObject.get_ptr());
				call_method(function, &env, this, 1, env.get_top_index());
			}

			if (ev.code == playStop || ev.code == playStreamNotFound)
			{
				get_root()->remove_listener(this);
			}
		}
	}

	// it is running in decoder thread
	void as_netstream::set_status(netstream_event_level level, netstream_event_code code)
	{
//		printf("push status: %s %s\n", level, code);
		stream_event ev;
		ev.level = level;
		ev.code = code;
		m_event.push(ev);
	}

	// it is running in decoder thread
	void as_netstream::close_stream()
	{
		set_video_data(NULL);

		m_vq.clear();
		m_aq.clear();

		if (m_VCodecCtx) avcodec_close(m_VCodecCtx);
		m_VCodecCtx = NULL;

		if (m_ACodecCtx) avcodec_close(m_ACodecCtx);
		m_ACodecCtx = NULL;

		if (m_FormatCtx) av_close_input_file(m_FormatCtx);
		m_FormatCtx = NULL;

		if (m_convert_ctx)
		{
			sws_freeContext(m_convert_ctx);
			m_convert_ctx = NULL;
		}
	}

	// it is running in decoder thread
	bool as_netstream::open_stream(const char* c_url)
	{
		// This registers all available file formats and codecs 
		// with the library so they will be used automatically when
		// a file with the corresponding format/codec is opened

		// Open video file
		// The last three parameters specify the file format, buffer size and format parameters;
		// by simply specifying NULL or 0 we ask libavformat to auto-detect the format 
		// and use a default buffer size

		if (av_open_input_file(&m_FormatCtx, c_url, NULL, 0, NULL) != 0)
		{
//			log_error("Couldn't open file '%s'\n", c_url);
			set_status(error, playStreamNotFound);
			return false;
		}

		// Next, we need to retrieve information about the streams contained in the file
		// This fills the streams field of the AVFormatContext with valid information
		if (av_find_stream_info(m_FormatCtx) < 0)
		{
			log_error("Couldn't find stream information from '%s'\n", c_url);
			return false;
		}

		// Find the first video & audio stream
		m_video_index = -1;
		m_audio_index = -1;
		for (int i = 0; i < m_FormatCtx->nb_streams; i++)
		{
			AVCodecContext* enc = m_FormatCtx->streams[i]->codec; 

			switch (enc->codec_type)
			{
				default:
					break;

				case CODEC_TYPE_AUDIO:

					if (m_audio_index < 0)
					{
						m_audio_index = i;
						m_audio_stream = m_FormatCtx->streams[i];
					}
					break;

				case CODEC_TYPE_VIDEO:

					if (m_video_index < 0) 
					{
						m_video_index = i;
						m_video_stream = m_FormatCtx->streams[i];
					}
					break;

				case CODEC_TYPE_DATA:
				case CODEC_TYPE_SUBTITLE:
				case CODEC_TYPE_UNKNOWN:
					break;
			}
		}

		if (m_video_index < 0)
		{
			log_error("Didn't find a video stream from '%s'\n", c_url);
			return false;
		}

		// Get a pointer to the codec context for the video stream
		m_VCodecCtx = m_FormatCtx->streams[m_video_index]->codec;

		// Find the decoder for the video stream
		AVCodec* pCodec = avcodec_find_decoder(m_VCodecCtx->codec_id);
		if (pCodec == NULL)
		{
			m_VCodecCtx = NULL;
			log_error("Decoder not found\n");
			return false;
		}

		// Open codec
		if (avcodec_open(m_VCodecCtx, pCodec) < 0)
		{
			m_VCodecCtx = NULL;
			log_error("Could not open codec\n");
			return false;
		}

		if (m_audio_index >= 0 && get_sound_handler() != NULL)
		{
			// Get a pointer to the audio codec context for the video stream
			m_ACodecCtx = m_FormatCtx->streams[m_audio_index]->codec;

			// Find the decoder for the audio stream
			AVCodec* pACodec = avcodec_find_decoder(m_ACodecCtx->codec_id);
			if (pACodec == NULL)
			{
				log_error("No available AUDIO decoder to process MPEG file: '%s'\n", c_url);
				return false;
			}

			// Open codec
			if (avcodec_open(m_ACodecCtx, pACodec) < 0)
			{
				log_error("Could not open AUDIO codec\n");
				return false;
			}
		}

		m_convert_ctx = sws_getContext(
			m_VCodecCtx->width, m_VCodecCtx->height, m_VCodecCtx->pix_fmt,
			m_VCodecCtx->width, m_VCodecCtx->height, PIX_FMT_RGBA32,
			SWS_BICUBIC, NULL, NULL, NULL);

		assert(m_convert_ctx);

		return true;
	}


	// it is running in sound mixer thread
	bool as_netstream::decode_audio(const AVPacket& pkt, Sint16** data, int* size)
	{
		bool ok = false;
		int frame_size;
		Uint8* decoder_buf = (Uint8*) malloc((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
		if (avcodec_decode_audio(m_ACodecCtx, (Sint16*) decoder_buf, &frame_size,	pkt.data, pkt.size) >= 0)
		{
			sound_handler* sh = get_sound_handler();
			if (sh)
			{
				sh->cvt(data, size, decoder_buf, frame_size, m_ACodecCtx->channels, m_ACodecCtx->sample_rate);
				ok = true;
			}
		}
		free(decoder_buf);
		return ok;
	}

	// it is running in decoder thread
	Uint8* as_netstream::decode_video(const AVPacket& pkt)
	{
		int got = 0;
		AVFrame frame;
		avcodec_decode_video(m_VCodecCtx, &frame, &got, pkt.data, pkt.size);
		if (got)
		{
			// get buf for rgba picture
			// will be freed later in update_video() 
			int size = avpicture_get_size(PIX_FMT_RGBA32, m_VCodecCtx->width, m_VCodecCtx->height);
			Uint8* data = (Uint8*) malloc(size);

			AVPicture picture1;
			avpicture_fill(&picture1, data, PIX_FMT_RGBA32, m_VCodecCtx->width, m_VCodecCtx->height);

			sws_scale(m_convert_ctx, frame.data, frame.linesize, 0, 0,
				picture1.data, picture1.linesize);

			return data;
		}
		return NULL;
	}

	int as_netstream::get_width() const
	{
		if (m_VCodecCtx)
		{
			return m_VCodecCtx->width;
		}
		return 0;
	}

	int as_netstream::get_height() const
	{
		if (m_VCodecCtx)
		{
			return m_VCodecCtx->height;
		}
		return 0;
	}

	// pass video data in video_stream_instance()
	Uint8* as_netstream::get_video_data()
	{
		tu_autolock locker(m_lock_video);
		Uint8* video_data = m_video_data;
		m_video_data = NULL;
		return video_data;
	}

	void as_netstream::set_video_data(Uint8* data)
	{
		tu_autolock locker(m_lock_video);
		if (m_video_data)
		{
			free(m_video_data);
		}
		m_video_data = data;
	}

	double as_netstream::get_duration() const
	{
		return (double) m_FormatCtx->duration / 1000000;
	}

	double as_netstream::get_bufferlength()
	{
		if (m_video_stream != NULL)
		{
			// hack,
			// TODO: take account of PTS (presentaion time stamp)
			return m_vq.size() * av_q2d(m_video_stream->codec->time_base);	// frame_delay
		}
		return 0;
	}

	void netstream_close(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);
		ns->close();
	}

	// flag:Boolean [optional] - A Boolean value specifying whether to pause play (true) or resume play (false).
	// If you omit this parameter, NetStream.pause() acts as a toggle: the first time it is called on a specified stream,
	// it pauses play, and the next time it is called, it resumes play.
	void netstream_pause(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);

		if (fn.nargs > 0)
		{
			bool pause = fn.arg(0).to_bool();
			if (pause == false && ns->m_status == as_netstream::PAUSE)	// play
			{
				ns->play(NULL);
			}
			else
			if (pause == true && ns->m_status == as_netstream::PLAY)	// pause
			{
				ns->m_status = as_netstream::PAUSE;
			}
		}
		else
		{
			// toggle mode
			if (ns->m_status == as_netstream::PAUSE)	// play
			{
				ns->play(NULL);
			}
			else
			if (ns->m_status == as_netstream::PLAY)	// pause
			{
				ns->m_status = as_netstream::PAUSE;
			}
		}
	}

	void netstream_play(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);

		if (fn.nargs < 1)
		{
			log_error("NetStream play needs args\n");
			return;
		}

		ns->play(fn.arg(0).to_tu_string());
	}

	// Seeks the keyframe closest to the specified number of seconds from the beginning
	// of the stream.
	void netstream_seek(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);

		if (fn.nargs < 1)
		{
			log_error("NetStream seek needs args\n");
			return;
		}

		ns->seek(fn.arg(0).to_number());
	}

	// public setBufferTime(bufferTime:Number) : Void
	// bufferTime:Number - The number of seconds of data to be buffered before 
	// Flash begins displaying data.
	// The default value is 0.1 (one-tenth of a second).
	void netstream_setbuffertime(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);

		if (fn.nargs < 1)
		{
			log_error("setBufferTime needs args\n");
			return;
		}

		ns->set_buffertime(fn.arg(0).to_number());
	}

	void netstream_time(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);
		fn.result->set_double(ns->time());
	}

	void netstream_buffer_length(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);
		fn.result->set_double(ns->get_bufferlength());
	}

	void netstream_buffer_time(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);
		fn.result->set_double(ns->get_buffertime());
	}

	void netstream_video_width(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);
		fn.result->set_int(ns->get_width());
	}

	void netstream_video_height(const fn_call& fn)
	{
		as_netstream* ns = cast_to<as_netstream>(fn.this_ptr);
		assert(ns);
		fn.result->set_int(ns->get_height());
	}

	void	as_global_netstream_ctor(const fn_call& fn)
	// Constructor for ActionScript class NetStream.
	{
		as_object* netstream = new as_netstream(fn.get_player());

		// properties
		netstream->builtin_member("time", as_value(netstream_time, as_value()));
		netstream->builtin_member("bufferLength", as_value(netstream_buffer_length, as_value()));
		netstream->builtin_member("bufferTime", as_value(netstream_buffer_time, as_value()));

		// methods
		netstream->builtin_member("close", netstream_close);
		netstream->builtin_member("pause", netstream_pause);
		netstream->builtin_member("play", netstream_play);
		netstream->builtin_member("seek", netstream_seek);
		netstream->builtin_member("setbuffertime", netstream_setbuffertime);

		// gameswf extension, video width & height
		netstream->builtin_member("_width", as_value(netstream_video_width, as_value()));
		netstream->builtin_member("_height", as_value(netstream_video_height, as_value()));

		fn.result->set_as_object(netstream);
	}


} // end of gameswf namespace

#else

#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_log.h"

namespace gameswf
{
	void as_global_netstream_ctor(const fn_call& fn)
	{
		log_error("video requires FFMPEG library\n");
	}
}

#endif
