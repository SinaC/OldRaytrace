//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Bitmap TGA file I/O
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "ImportTGABitmap.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImportTGABitmap ( char *_filename, ARGBPixelBuffer &_buffer )
{
  TGA_Composition			composition;
  TGA_Palette				palette;

  if(!ReadTGAFile(_filename,composition))
    return false;

  switch(composition.header->imagetype)
    {
    default:
    case IMAGETYPE_NOIMAGE:
      FreeTGAFile				(composition);
      return false;
      break;

    case IMAGETYPE_COLORMAPPEDIMAGE:
    case IMAGETYPE_ENCODEDCOLORMAPPEDIMAGE:
      if(!DecodePalette(composition,palette))
	return false;
      if(!DecodePixPalette(composition,_buffer,palette))
	return false;
      break;

    case IMAGETYPE_TRUECOLORIMAGE:
    case IMAGETYPE_ENCODEDTRUECOLORIMAGE:
      if(!DecodePixARGB(composition,_buffer))
	return false;
      break;

    case IMAGETYPE_MONOCHROMEIMAGE:
    case IMAGETYPE_ENCODEDMONOCHROMEIMAGE:
      if(!DecodePixIntensity(composition,_buffer))
	return false;
      break;
    }

  FreeTGAFile				(composition);

  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ReadTGAFile ( char *_filename, TGA_Composition &_composition )
{
  /*
    u32					a,count;
  */
  FILE				*file;
  u32					filesize;
  u8					*data;


  // Check
  if(!_filename)
    return false;

  // Open & get file size file
  file		=	fopen(_filename,"rb");
  if(!file)	
    return false;
  fseek			(file,0,SEEK_END);
  filesize	=	ftell(file);
  fseek			(file,0,SEEK_SET);

  // Read raw data & close file
  data		=	(u8 *) malloc(filesize);
  if(!data)
    {
      fclose		(file);
      return false;
    }
  fread			(data,filesize,1,file);
  fclose			(file);

  // Fill-in composition
  _composition.header								=	NULL;
  _composition.imageid							=	NULL;
  _composition.colormapdata						=	NULL;
  _composition.imagedata							=	NULL;
  /*
    _composition.devdirentries.SetSize					(0);	
  */
  _composition.devdirectory						=	NULL;
  _composition.extension							=	NULL;
  _composition.scanlinetable						=	NULL;
  _composition.postagestampimage					=	NULL;
  _composition.colorcorrectiontable				=	NULL;
  _composition.footer								=	NULL;
	
  // Read & parse header
  _composition.header								=	(TGA_Header *)data;
  if(_composition.header->idlength)
    _composition.imageid						=	(u8 *)(data + sizeof(TGA_Header));
  if(		(_composition.header->imagetype == IMAGETYPE_COLORMAPPEDIMAGE)
		||	(_composition.header->imagetype == IMAGETYPE_ENCODEDCOLORMAPPEDIMAGE) )
    _composition.colormapdata					=	(u8 *)(data + sizeof(TGA_Header) + _composition.header->idlength);
	
  if(_composition.header->imagetype != IMAGETYPE_NOIMAGE)
    {
      _composition.imagedata							=	(u8 *)(data + sizeof(TGA_Header) + _composition.header->idlength);
      if(_composition.colormapdata)
	{
	  switch(_composition.header->colormapdepth)
	    {
	    case 15:
	    case 16:
	      _composition.imagedata				+=	_composition.header->colormaplength * 2;
	      break;

	    case 24:
	      _composition.imagedata				+=	_composition.header->colormaplength * 3;
	      break;

	    case 32:
	      _composition.imagedata				+=	_composition.header->colormaplength * 4;
	      break;
	    }
	}
    }

  // Check if version 2.0 & parse footer
  _composition.footer								=	(TGA_Footer *)(data + filesize - sizeof(TGA_Footer));
  if(strcmp(_composition.footer->signature,"TRUEVISION-XFILE."))
    _composition.footer							=	NULL;
  else
    {
      // Get pointer to extension
      if(_composition.footer->extensionoffset)
	{
	  _composition.extension					=	(TGA_Extension *)(data + _composition.footer->extensionoffset);

	  if(_composition.extension->scanlineoffset)
	    _composition.scanlinetable			=	(u8 *)(data + _composition.extension->scanlineoffset);
	  if(_composition.extension->postagestampoffset)
	    _composition.postagestampimage		=	(u8 *)(data + _composition.extension->postagestampoffset);
	  if(_composition.extension->colorcorrectionoffset)
	    _composition.colorcorrectiontable	=	(u16 *)(data + _composition.extension->colorcorrectionoffset);
	}

      // Get pointer to developper directory
      if(_composition.footer->devdirectoryoffset)
	{
	  _composition.devdirectory				=	(TGA_DevDirectory *)(data + _composition.footer->devdirectoryoffset);
	  /*			
			_composition.devdirentries.SetSize			(_composition.devdirectory->numtags);

			count									=	_composition.devdirectory->numtags;
			for(a=0; a<count; a++)
			{
			_composition.devdirentries[a].tag	=	*((u16 *)(data + _composition.footer->devdirectoryoffset + 2 + 10*a ));
			_composition.devdirentries[a].data	=	data + *((u32 *)(data + _composition.footer->devdirectoryoffset + 2 + 10*a + 2 ));
			_composition.devdirentries[a].size	=	*((u32 *)(data + _composition.footer->devdirectoryoffset + 2 + 10*a + 6 ));
			}
	  */			
	}
    }

  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FreeTGAFile ( TGA_Composition &_composition )
{
  free												(_composition.header);				

  _composition.header								=	NULL;
  _composition.imageid							=	NULL;
  _composition.colormapdata						=	NULL;
  _composition.imagedata							=	NULL;
  /*
    _composition.devdirentries.SetSize					(0);	
  */
  _composition.devdirectory						=	NULL;
  _composition.extension							=	NULL;
  _composition.scanlinetable						=	NULL;
  _composition.postagestampimage					=	NULL;
  _composition.colorcorrectiontable				=	NULL;
  _composition.footer								=	NULL;

  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DecodePalette ( TGA_Composition &_composition, TGA_Palette &_palette )
{
  u32		a,count;
  u8		*p8;
  u16		*p16;
  u32		*p32;

  if(!_composition.colormapdata)
    return false;

  // Clean
  for(a=0; a<256; a++)
    _palette.col[a]	=	0xff000000;

  // Read palette
  count	=	_composition.header->colormaplength;
  switch(_composition.header->colormapdepth)
    {
    case 15:
      p16	=	(u16 *)(_composition.colormapdata);
      p32	=	_palette.col + _composition.header->colormapstart;
      for(a=0; a<count; a++)
	{
	  *p32	=	0xff000000;
	  *p32	|=	(((*p16)>>10)&0x1f)<<(16+3);
	  *p32	|=	(((*p16)>>5)&0x1f)<<(8+3);
	  *p32	|=	(((*p16)>>0)&0x1f)<<(0+3);
	  p32++;
	  p16++;
	}
      break;

    case 16:
      p16	=	(u16 *)(_composition.colormapdata);
      p32	=	_palette.col + _composition.header->colormapstart;
      for(a=0; a<count; a++)
	{
	  *p32	=	((*p16)&0x8000)?0xff000000:0;
	  *p32	|=	(((*p16)>>10)&0x1f)<<(16+3);
	  *p32	|=	(((*p16)>>5)&0x1f)<<(8+3);
	  *p32	|=	(((*p16)>>0)&0x1f)<<(0+3);
	  p32++;
	  p16++;
	}
      break;

    case 24:
      p8	=	(u8 *)(_composition.colormapdata);
      p32	=	_palette.col + _composition.header->colormapstart;
      for(a=0; a<count; a++)
	{
	  *p32	=	0xff000000;
	  *p32	|=	(*p8);
	  *p32	|=	(*(p8+1))<<8;
	  *p32	|=	(*(p8+2))<<16;
	  p32++;
	  p8		+=	3;
	}
      break;

    case 32:
      p32	=	_palette.col + _composition.header->colormapstart;
      memcpy(p32,_composition.colormapdata,4*count);
      break;

    default:
      return false;
      break;
    }

  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DecodePixARGB ( TGA_Composition &_composition, ARGBPixelBuffer &_argbbuffer )
{
  u32		a,b,count,numpix;
  u8		*p8;
  u16		*p16;
  u32		*p32;

  u8		*rledata;
  u32		pixel;

  numpix				=	_composition.header->width * _composition.header->height;
  _argbbuffer.SetSize		(_composition.header->width,_composition.header->height);
		

  switch(_composition.header->imagetype)
    {
    case IMAGETYPE_TRUECOLORIMAGE:
      switch(_composition.header->pixeldepth)
	{
	case 15:
	  p16		=	(u16 *)_composition.imagedata;
	  p32		=	_argbbuffer.pixels;
	  for(a=0; a<numpix; a++,p16++,p32++)
	    {
	      *p32	=	0xff000000;
	      *p32	|=	(((*p16)>>10)&0x1f)<<(16+3);
	      *p32	|=	(((*p16)>>5)&0x1f)<<(8+3);
	      *p32	|=	(((*p16)>>0)&0x1f)<<(0+3);
	    }
	  break;

	case 16:
	  p16		=	(u16 *)_composition.imagedata;
	  p32		=	_argbbuffer.pixels;
	  for(a=0; a<numpix; a++,p16++,p32++)
	    {
	      *p32	=	((*p16)&0x8000)?0xff000000:0;
	      *p32	|=	(((*p16)>>10)&0x1f)<<(16+3);
	      *p32	|=	(((*p16)>>5)&0x1f)<<(8+3);
	      *p32	|=	(((*p16)>>0)&0x1f)<<(0+3);
	    }
	  break;

	case 24:
	  p8		=	_composition.imagedata;
	  p32		=	_argbbuffer.pixels;
	  for(a=0; a<numpix; a++,p8+=3,p32++)
	    {
	      *p32	=	0xff000000;
	      *p32	|=	(*p8);
	      *p32	|=	(*(p8+1))<<8;
	      *p32	|=	(*(p8+2))<<16;
	    }
	  break;

	case 32:
	  memcpy	(_argbbuffer.pixels,_composition.imagedata,numpix*4);
	  break;

	default:
	  return false;
	  break;
	}
      break;

    case IMAGETYPE_ENCODEDTRUECOLORIMAGE:
      rledata			=	_composition.imagedata;
      p32				=	_argbbuffer.pixels;
      a				=	0;
      switch(_composition.header->pixeldepth)
	{
	case 15:
	  while(a<numpix)
	    {
	      count			=	((*rledata)&0x7f)+1;
	      if((*rledata)&0x80)
		{
		  p16			=	(u16 *)(rledata+1);
		  pixel		=	0xff000000;
		  pixel		|=	(((*p16)>>10)&0x1f)<<(16+3);
		  pixel		|=	(((*p16)>>5)&0x1f)<<(8+3);
		  pixel		|=	(((*p16)>>0)&0x1f)<<(0+3);
		  for(b=0; b<count; b++,p32++)
		    *p32	=	pixel;

		  rledata		=	(u8 *)(p16+1);
		  a			+=	count;

		}
	      else
		{
		  p16	=	(u16 *)(rledata+1);
		  for(b=0; b<count; b++,p16++,p32++)
		    {
		      *p32	=	0xff000000;
		      *p32	|=	(((*p16)>>10)&0x1f)<<(16+3);
		      *p32	|=	(((*p16)>>5)&0x1f)<<(8+3);
		      *p32	|=	(((*p16)>>0)&0x1f)<<(0+3);
		    }

		  rledata		=	(u8 *)p16;
		  a			+=	count;
		}
	    }
	  break;

	case 16:
	  while(a<numpix)
	    {
	      count			=	((*rledata)&0x7f)+1;
	      if((*rledata)&0x80)
		{
		  p16			=	(u16 *)(rledata+1);
		  pixel		=	((*p16)&0x8000)?0xff000000:0;
		  pixel		|=	(((*p16)>>10)&0x1f)<<(16+3);
		  pixel		|=	(((*p16)>>5)&0x1f)<<(8+3);
		  pixel		|=	(((*p16)>>0)&0x1f)<<(0+3);
		  for(b=0; b<count; b++,p32++)
		    *p32	=	pixel;

		  rledata		=	(u8 *)(p16+1);
		  a			+=	count;

		}
	      else
		{
		  p16	=	(u16 *)(rledata+1);
		  for(b=0; b<count; b++,p16++,p32++)
		    {
		      *p32	=	((*p16)&0x8000)?0xff000000:0;
		      *p32	|=	(((*p16)>>10)&0x1f)<<(16+3);
		      *p32	|=	(((*p16)>>5)&0x1f)<<(8+3);
		      *p32	|=	(((*p16)>>0)&0x1f)<<(0+3);
		    }

		  rledata		=	(u8 *)p16;
		  a			+=	count;
		}
	    }
	  break;

	case 24:
	  while(a<numpix)
	    {
	      count			=	((*rledata)&0x7f)+1;
	      if((*rledata)&0x80)
		{
		  p8			=	(u8 *)(rledata+1);
		  pixel		=	0xff000000;
		  pixel		|=	(*p8);
		  pixel		|=	(*(p8+1))<<8;
		  pixel		|=	(*(p8+2))<<16;
		  for(b=0; b<count; b++,p32++)
		    *p32	=	pixel;

		  rledata		=	(u8 *)(p8+3);
		  a			+=	count;

		}
	      else
		{
		  p8			=	(u8 *)(rledata+1);
		  for(b=0; b<count; b++,p8+=3,p32++)
		    {
		      *p32	=	0xff000000;
		      *p32	|=	(*p8);
		      *p32	|=	(*(p8+1))<<8;
		      *p32	|=	(*(p8+2))<<16;
		    }

		  rledata		=	(u8 *)p8;
		  a			+=	count;
		}
	    }
	  break;

	case 32:
	  while(a<numpix)
	    {
	      count			=	((*rledata)&0x7f)+1;
	      if((*rledata)&0x80)
		{
		  pixel		=	*((u32 *)(rledata+1));
		  for(b=0; b<count; b++,p32++)
		    *p32	=	pixel;

		  rledata		+=	5;
		  a			+=	count;

		}
	      else
		{
		  for(b=0; b<count; b++,p32++)
		    *p32	=	*((u32 *)(rledata+1+(b<<2)));

		  rledata		=	rledata+1+(b<<2);
		  a			+=	count;
		}
	    }

	  break;

	default:
	  return false;
	  break;
	}
      break;
    }

  if(_composition.header->originx)
    _argbbuffer.FlipHorizontal();

  if(!_composition.header->originy)
    _argbbuffer.FlipVertical();

  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DecodePixIntensity ( TGA_Composition &_composition, ARGBPixelBuffer &_argbbuffer )
{
  u32		a,b,count,numpix;
  u8		*p8;
  u32		*p32,pixel;
  u8		*rledata;

  numpix				=	_composition.header->width * _composition.header->height;
  _argbbuffer.SetSize		(_composition.header->width,_composition.header->height);
		

  switch(_composition.header->imagetype)
    {
    case IMAGETYPE_MONOCHROMEIMAGE:
      p8		=	_composition.imagedata;
      p32		=	_argbbuffer.pixels;
      for(a=0; a<numpix; a++,p8++,p32++)
	*p32	=	0xff000000|((*p8)<<16)|((*p8)<<8)|(*p8);
      break;

    case IMAGETYPE_ENCODEDMONOCHROMEIMAGE:
      rledata			=	_composition.imagedata;
      p32				=	_argbbuffer.pixels;
      a				=	0;

      while(a<numpix)
	{
	  count			=	((*rledata)&0x7f)+1;
	  if((*rledata)&0x80)
	    {
	      p8			=	(u8 *)(rledata+1);
	      pixel		=	0xff000000|((*p8)<<16)|((*p8)<<8)|(*p8);
	      for(b=0; b<count; b++,p32++)
		*p32	=	pixel;

	      rledata		=	(u8 *)(p8+1);
	      a			+=	count;

	    }
	  else
	    {
	      p8			=	(u8 *)(rledata+1);
	      for(b=0; b<count; b++,p8++,p32++)
		*p32	=	0xff000000|((*p8)<<16)|((*p8)<<8)|(*p8);

	      rledata		=	(u8 *)p8;
	      a			+=	count;
	    }
	}
      break;
    }

  if(_composition.header->originx)
    _argbbuffer.FlipHorizontal();

  if(!_composition.header->originy)
    _argbbuffer.FlipVertical();

  return true;

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DecodePixPalette ( TGA_Composition &_composition, ARGBPixelBuffer &_argbbuffer, TGA_Palette &_palette )
{
  u32		a,b,count,numpix;
  u8		*p8;
  u32		*p32,pixel;
  u8		*rledata;

  numpix				=	_composition.header->width * _composition.header->height;
  _argbbuffer.SetSize		(_composition.header->width,_composition.header->height);
		

  switch(_composition.header->imagetype)
    {
    case IMAGETYPE_COLORMAPPEDIMAGE:
      p8		=	_composition.imagedata;
      p32		=	_argbbuffer.pixels;
      for(a=0; a<numpix; a++,p8++,p32++)
	*p32	=	_palette.col[*p8];
      break;

    case IMAGETYPE_ENCODEDCOLORMAPPEDIMAGE:
      rledata			=	_composition.imagedata;
      p32				=	_argbbuffer.pixels;
      a				=	0;

      while(a<numpix)
	{
	  count			=	((*rledata)&0x7f)+1;
	  if((*rledata)&0x80)
	    {
	      p8			=	(u8 *)(rledata+1);
	      pixel		=	_palette.col[*p8];
	      for(b=0; b<count; b++,p32++)
		*p32	=	pixel;

	      rledata		=	(u8 *)(p8+1);
	      a			+=	count;

	    }
	  else
	    {
	      p8			=	(u8 *)(rledata+1);
	      for(b=0; b<count; b++,p8++,p32++)
		*p32	=	_palette.col[*p8];

	      rledata		=	(u8 *)p8;
	      a			+=	count;
	    }
	}
      break;
    }

  if(_composition.header->originx)
    _argbbuffer.FlipHorizontal();

  if(!_composition.header->originy)
    _argbbuffer.FlipVertical();

  return true;
}
