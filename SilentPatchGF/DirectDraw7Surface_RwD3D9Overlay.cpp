#include "DirectDraw7Surface_RwD3D9Overlay.h"
#include "DirectDraw7Surface_RwD3D9RT.h"

#include <cassert>

static int32_t DD7LockModeToRWLockMode( DWORD dwFlags )
{
	int32_t lockMode = 0;
	if ( (dwFlags & DDLOCK_READONLY) ) lockMode = rwRASTERLOCKREAD;
	else if ( (dwFlags & DDLOCK_WRITEONLY) ) lockMode = rwRASTERLOCKWRITE|rwRASTERLOCKNOFETCH;

	return lockMode;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
	if ( lpDestRect != nullptr || lpDDSurfaceDesc == nullptr ) return DDERR_INVALIDPARAMS;

	uint8_t* pixels = RwRasterLock( m_raster, 0, DD7LockModeToRWLockMode( dwFlags ) );
	lpDDSurfaceDesc->lpSurface = pixels;

	return pixels != nullptr ? DD_OK : DDERR_OUTOFMEMORY;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::Unlock(LPRECT lpRect)
{
	if ( lpRect != nullptr ) return DDERR_INVALIDRECT;

	return RwRasterUnlock( m_raster ) != nullptr ? DD_OK : DDERR_NOTLOCKED;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE7 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
	if ( (dwFlags & DDOVER_SHOW) != 0 )
	{
		// TODO: Rects
		DirectDraw7Surface_RwD3D9RT* rt = static_cast<DirectDraw7Surface_RwD3D9RT*>(lpDDDestSurface);
		rt->AddOverlayToQueue( m_raster, m_verts );
		return DD_OK;
	}

	//assert( !"Unimplemented UpdateOverlay flag!" );
	return E_NOTIMPL;
}

DirectDraw7Surface_RwD3D9Overlay::DirectDraw7Surface_RwD3D9Overlay(int32_t width, int32_t height)
{
	m_raster = RwRasterCreate( width, height, 0, rwRASTERFORMAT8888|rwRASTERTYPETEXTURE );
	//m_raster = RwD3D9RasterCreate( width, height, D3DFMT_A8L8, rwRASTERTYPETEXTURE );

	auto* vertices = m_verts;

	size_t index = 0;
	vertices[index].x = 640.0f;
	vertices[index].y = 0.0f;
	vertices[index].z = 0.0f;
	vertices[index].rhw = 0.0f;
	vertices[index].u = 1.0f;
	vertices[index].v = 0.0f;
	vertices[index].emissiveColor = 0xFFFFFFFF;
	index++;

	vertices[index].x = 0.0f;
	vertices[index].y = 480.0f;
	vertices[index].z = 0.0f;
	vertices[index].rhw = 0.0f;
	vertices[index].u = 0.0f;
	vertices[index].v = 1.0f;
	vertices[index].emissiveColor = 0xFFFFFFFF;
	index++;

	vertices[index].x = 0.0f;
	vertices[index].y = 0.0f;
	vertices[index].z = 0.0f;
	vertices[index].rhw = 0.0f;
	vertices[index].u = 0.0f;
	vertices[index].v = 0.0f;
	vertices[index].emissiveColor = 0xFFFFFFFF;
	index++;

	vertices[index].x = 0.0f;
	vertices[index].y = 480.0f;
	vertices[index].z = 0.0f;
	vertices[index].rhw = 0.0f;
	vertices[index].u = 0.0f;
	vertices[index].v = 1.0f;
	vertices[index].emissiveColor = 0xFFFFFFFF;
	index++;

	vertices[index].x = 640.0f;
	vertices[index].y = 480.0f;
	vertices[index].z = 0.0f;
	vertices[index].rhw = 0.0f;
	vertices[index].u = 1.0f;
	vertices[index].v = 1.0f;
	vertices[index].emissiveColor = 0xFFFFFFFF;
	index++;
}

DirectDraw7Surface_RwD3D9Overlay::~DirectDraw7Surface_RwD3D9Overlay()
{
	// TODO: Raster destroy
}