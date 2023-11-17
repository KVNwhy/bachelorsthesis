using UnrealBuildTool;
using System;
public class OpenUVR : ModuleRules
{
	public OpenUVR(ReadOnlyTargetRules Target) : base(Target) //hier
	{
		Type = ModuleType.External;
		string OUVR_path = "$(EngineDir)/Binaries/ThirdParty/OpenUVR/libs/";
		Console.WriteLine(Target.UEThirdPartyBinariesDirectory + "OpenUVR/libs/"); //hier
		//PublicLibraryPaths.Add();

		PublicSystemIncludePaths.Add("$(EngineDir)/Source/ThirdParty/OpenUVR/");

		//PublicLibraryPaths.Add(Target.UEThirdPartyBinariesDirectory + "OpenUVR/libs/"); //hier
		//PublicLibraryPaths.Add("/usr/lib/x86_64-linux-gnu/"); 
		PublicSystemLibraryPaths.Add("/usr/lib/x86_64-linux-gnu/"); 
		PublicSystemLibraryPaths.Add("/usr/local/lib/");
		
		PublicAdditionalLibraries.Add(Target.UEThirdPartyBinariesDirectory + "OpenUVR/libs/"+"libopenuvr.so"); //Hier
		RuntimeDependencies.Add(OUVR_path + "libopenuvr.so");

		PublicSystemLibraries.Add("cuda");
		//RuntimeDependencies.Add(new RuntimeDependency("/usr/lib/x86_64-linux-gnu/libcuda.so"));

		PublicSystemLibraries.Add("glut");
		PublicSystemLibraries.Add("python3.5m");
		//RuntimeDependencies.Add(new RuntimeDependency("/usr/lib/x86_64-linux-gnu/libglut.so"));


		// PublicAdditionalLibraries.Add("pulse");
		// PublicAdditionalLibraries.Add("pulse-simple");
		
		
		
		PublicAdditionalLibraries.Add("ass");
		PublicSystemLibraries.Add("SDL2-2.0");
		PublicSystemLibraries.Add("sndio");
		PublicSystemLibraries.Add("asound");
		PublicSystemLibraries.Add("vdpau");
		PublicSystemLibraries.Add("dl");
		PublicSystemLibraries.Add("va");
		PublicSystemLibraries.Add("va-drm");
		PublicSystemLibraries.Add("Xext");
		PublicSystemLibraries.Add("xcb-shm");
		PublicSystemLibraries.Add("xcb-xfixes");
		PublicSystemLibraries.Add("xcb-shape");
		PublicSystemLibraries.Add("xcb");
		PublicSystemLibraries.Add("Xv");
		PublicSystemLibraries.Add("freetype");
		PublicSystemLibraries.Add("va-x11");
		PublicSystemLibraries.Add("X11");
		PublicSystemLibraries.Add("pthread");
		PublicSystemLibraries.Add("m");
		PublicSystemLibraries.Add("z");

		PublicAdditionalLibraries.Add("avdevice");
		PublicAdditionalLibraries.Add("avfilter");
		PublicAdditionalLibraries.Add("avformat");
		PublicAdditionalLibraries.Add("avcodec");
		PublicAdditionalLibraries.Add("avutil");
		PublicAdditionalLibraries.Add("swscale");
		PublicAdditionalLibraries.Add("swresample");
		RuntimeDependencies.Add(OUVR_path + "libavdevice.so");
		RuntimeDependencies.Add(OUVR_path + "libavfilter.so");
		RuntimeDependencies.Add(OUVR_path + "libavformat.so");
		RuntimeDependencies.Add(OUVR_path + "libavcodec.so");
		RuntimeDependencies.Add(OUVR_path + "libavutil.so");
		RuntimeDependencies.Add(OUVR_path + "libswscale.so");
		RuntimeDependencies.Add(OUVR_path + "libswresample.so");


		PublicSystemLibraries.Add("python3.5m");
		RuntimeDependencies.Add("/usr/lib/x86_64-linux-gnu/libpython3.5m.so.1.0");
	}
}

//-L/usr/local/cuda/lib64 $(OPENUVR_FFMPEG_LIB_DIR)/../../src/libopenuvr.so -lcuda -lglut $(OPENUVR_FFMPEG_LIB_DIR)/libavdevice.so $(OPENUVR_FFMPEG_LIB_DIR)/libavfilter.so $(OPENUVR_FFMPEG_LIB_DIR)/libavformat.so $(OPENUVR_FFMPEG_LIB_DIR)/libavcodec.so $(OPENUVR_FFMPEG_LIB_DIR)/libavutil.so $(OPENUVR_FFMPEG_LIB_DIR)/libswscale.so $(OPENUVR_FFMPEG_LIB_DIR)/libswresample.so -lnppicc -lnppig -lnppc -lass -lSDL2-2.0 -lsndio -lasound -lvdpau -ldl -lva -lva-drm -lXext -lxcb-shm -lxcb-xfixes -lxcb-shape -lxcb -lXv -lfreetype $(OPENUVR_FFMPEG_LIB_DIR)/libpostproc.so -lva-x11 -lX11 -lpthread -lm -lz
