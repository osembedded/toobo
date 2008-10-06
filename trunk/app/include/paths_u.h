/**
 * [paths_u.h] [Part of the TOOBO Project]
 * 
 * Copyright 2008 Vijay Jayaraman (osembedded@gmail.com) 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Lesser General Public License for more details. 
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * Purpose:
 * This file contains file/dir path information that is 'global' to the
 * system.
 */

#ifndef __PATHS_U__
#define __PATHS_U__

/* Log path for various process */
#define LOG_PIPE_ROOT                "/logpipes/"
#define STARTUP_MGR_LOG_PIPE_PATH    (LOG_PIPE_ROOT"startupMgr")
#define SCREEN_MGR_LOG_PIPE_PATH     (LOG_PIPE_ROOT"screenMgr")
#define INPUT_MGR_LOG_PIPE_PATH      (LOG_PIPE_ROOT"inputMgr")
#define SOUND_MGR_LOG_PIPE_PATH      (LOG_PIPE_ROOT"soundMgr")
#define CTRL_MGR_LOG_PIPE_PATH       (LOG_PIPE_ROOT"ctrlMgr")
#define SHUTDOWN_MGR_LOG_PIPE_PATH   (LOG_PIPE_ROOT"shutdownMgr")
#define STANDBY_MGR_LOG_PIPE_PATH    (LOG_PIPE_ROOT"standbyMgr")


#endif /* __PATHS_U__ */
