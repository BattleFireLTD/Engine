#pragma once

enum MemoryLabel{
#define MEMORY_LABEL(Name) kMem##Name##Id ,
	MEMORY_LABEL(Default)
	MEMORY_LABEL(VertexData)
	MEMORY_LABEL(IndexData)
	MEMORY_LABEL(Geometry)
	MEMORY_LABEL(Texture)
	MEMORY_LABEL(Shader)
	MEMORY_LABEL(GfxDevice)
	MEMORY_LABEL(GfxThread)
	MEMORY_LABEL(Animation)
	MEMORY_LABEL(Mesh)
	MEMORY_LABEL(Audio)
	MEMORY_LABEL(AudioData)
	MEMORY_LABEL(AudioProcessing)
	MEMORY_LABEL(Font)
	MEMORY_LABEL(Serialization)
	MEMORY_LABEL(IO)
	MEMORY_LABEL(IO2)
	MEMORY_LABEL(ThreadStack)
	MEMORY_LABEL(Renderer)
	MEMORY_LABEL(Transform)
	MEMORY_LABEL(Editor)
	MEMORY_LABEL(Profiler)
	MEMORY_LABEL(MemoryAllocator)
	MEMORY_LABEL(Particle)
	MEMORY_LABEL(System)
	MEMORY_LABEL(Material)
	MEMORY_LABEL(Script)
	MEMORY_LABEL(AssetBundle)
#undef MEMORY_LABEL
	kMemNormalLabelCount,
	kMemLabelCount
};