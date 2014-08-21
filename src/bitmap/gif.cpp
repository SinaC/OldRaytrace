#include "bitmap.h"

//--TODO--bool TBitmap::loadGIF( const char *filename ) {
//--TODO--  FILE *fp;
//--TODO--  if ( ( fp = fopen( filename, "rb" ) ) == NULL )
//--TODO--    return false;
//--TODO--
//--TODO--  u8 GIFHeader[13];
//--TODO--  
//--TODO--  if ( fread( GIFHeader, 1, 13, fp ) != 13 )
//--TODO--    return false;
//--TODO--
//--TODO--  // vérification "GIF" et "89a"
//--TODO--  if ( GIFHeader[0]!='G' || GIFHeader[1]!='I' || GIFHeader[2]!='F' ||
//--TODO--       GIFHeader[3]!='8' || GIFHeader[4]!='9' || GIFHeader[5]!='a' )
//--TODO--    return false;
//--TODO--
//--TODO--  // trouve le nombre de couleur de l'image
//--TODO--  u32 nbCouleur=1<< ( ( ( GIFHeader[10] & 0x70 ) >> 4 ) + 1 );
//--TODO--
//--TODO--  // lecture du bloc descripteur d'écran
//--TODO--  u32 ecranW= GIFHeader[6] + (GIFHeader[7]<<8);
//--TODO--  u32 ecranH = GIFHeader[8] + (GIFHeader[9]<<8);
//--TODO--  bool isPaletteGlobale = GIFHeader[10] & 0x80;
//--TODO--  u32 coulFond = GIFHeader[11];
//--TODO--
//--TODO--  // lit la palette globale
//--TODO--  u8 palette[768];
//--TODO--  if ( isPaletteGlobale ) {
//--TODO--    if ( !fread( palette, 1, nbCouleur*3, fp  ) == nbCouleur*3 )
//--TODO--      return false;
//--TODO--  }
//--TODO--
//--TODO--  // cherche le bloc descripteur d'image
//--TODO--  // saute les blocs d'extensions
//--TODO--  while ( fread( &buf, 1, 1==0x21 )
//--TODO--    // saute le bloc : taille du bloc = cFichier[2]+1
//--TODO--    cFichier+=4+cFichier[2];
//--TODO--
//--TODO--  if ( cFichier[0]!=0x2C )
//--TODO--    throw("Impossible de trouver le bloc descripteur d'image");
//--TODO--
//--TODO--  // lecture du bloc descripteur d'image
//--TODO--  imageW=cFichier[5]+(cFichier[6]<<8);
//--TODO--  imageH=cFichier[7]+(cFichier[8]<<8);
//--TODO--  isPaletteLocale=cFichier[10]&0x80;
//--TODO--  return cFichier+10;		// taille du bloc = 10
//--TODO--}
//--TODO--
//--TODO--Gif::Gif(char *name) : image(0)
//--TODO--{
//--TODO--    // Lit la palette locale si existe
//--TODO--    if ( isPaletteLocale )
//--TODO--      cFichier=LirePalette(palette,cFichier);
//--TODO--    // création de l'image
//--TODO--    image = new uchar[imageW*imageH];
//--TODO--    // décodage
//--TODO--    DecodeLZW(cFichier);
//--TODO--    if ( fichier )
//--TODO--      delete fichier;
//--TODO--}
//--TODO--
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Ouvre le fichier et test si la version est correcte
//--TODO--// Charge également les informations du bloc descripteur d'écran
//--TODO--// renvoi un pointeur sur les données du fichier
//--TODO--uchar* Gif::Ouvrir(char* name)
//--TODO--{
//--TODO--  HANDLE hFile;
//--TODO--  // ouvre le fichier en lecture
//--TODO--  hFile=CreateFile(name,GENERIC_READ,FILE_SHARE_READ,
//--TODO--		   NULL,OPEN_EXISTING,0,0);
//--TODO--  if ( hFile==INVALID_HANDLE_VALUE )
//--TODO--    throw("Impossible de lire le fichier");
//--TODO--
//--TODO--  // trouve la taille du fichier
//--TODO--  int taille=GetFileSize(hFile,NULL);
//--TODO--  if ( taille==-1 )
//--TODO--    throw("Impossible de trouver la taille du fichier");
//--TODO--  uchar* fichier = new uchar[taille];
//--TODO--
//--TODO--  // lecture du fichier
//--TODO--  DWORD numByteRead;
//--TODO--  if ( !ReadFile(hFile,fichier,taille,&numByteRead,NULL) )
//--TODO--    throw("Erreur de lecture");
//--TODO--
//--TODO--  // ferme le fichier
//--TODO--  CloseHandle(hFile);
//--TODO--
//--TODO--  // vérification "GIF" et "89a"
//--TODO--  if ( fichier[0]!='G' || fichier[1]!='I' || fichier[2]!='F' ||
//--TODO--       fichier[3]!='8' || fichier[4]!='9' || fichier[5]!='a' )
//--TODO--    throw("Image gif non valide");
//--TODO--
//--TODO--  // trouve le nombre de couleur de l'image
//--TODO--  nbCouleur=1<<(((fichier[10]&0x70)>>4)+1);
//--TODO--
//--TODO--  // lecture du bloc descripteur d'écran
//--TODO--  ecranW=fichier[6]+(fichier[7]<<8);
//--TODO--  ecranH=fichier[8]+(fichier[9]<<8);
//--TODO--  isPaletteGlobale=fichier[10]&0x80;
//--TODO--  coulFond=fichier[11];
//--TODO--  return fichier;
//--TODO--}
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Lit le block descripteur d'image
//--TODO--uchar* Gif::InfoBlock(uchar* cFichier)
//--TODO--{
//--TODO--}
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Decode l'image
//--TODO--void Gif::DecodeLZW(uchar* cFichier)
//--TODO--{
//--TODO--  // lit la taille du code
//--TODO--  int tailleCode=cFichier[0];
//--TODO--  cFichier++;
//--TODO--
//--TODO--  // Initialisation pour le decodage LZW
//--TODO--  int codeClear=1<<tailleCode;
//--TODO--  int codeFin=1<<tailleCode;
//--TODO--  int abSuffx[4096];		// alphabet suffixe et prefixe
//--TODO--  int abPrefx[4096];		// prefixe codé, suffixe concret
//--TODO--  int abPos=codeFin+1;	  	// position des nouvelles chaînes dans l'alphabet
//--TODO--  uchar pileDecode[1281];	// pile de decodage
//--TODO--  int posPileDec=0;			// position dans la pile de decodage
//--TODO--  int tempPosPileDec;		// sauvegarde temporaire de la position de la pile
//--TODO--  int nbBit=tailleCode+1;	// nombre de bit de l'alphabet
//--TODO--  int maxCode=1<<nbBit;	// code maxi pour le nombre de bits courant
//--TODO--  int curCode;				// code courant
//--TODO--  int oldCode=-1;			// ancien code
//--TODO--  int tempCode;				// sauvegarde temporaire
//--TODO--  int restBit=0;				// bits restant dans le dernier octet lu
//--TODO--  int lByte=0;				// dernier octet lu
//--TODO--  int maskBit=0xFFFFFFFF>>(32-nbBit);		// mask pour retrouver le code courant
//--TODO--  bool isCasSpecial=false;	// indique un cas special
//--TODO--
//--TODO--  // Initialisation pour le chargement des données par bloc
//--TODO--  int nbBuff=0;					// nombre d'éléments restant dans un bloc
//--TODO--
//--TODO--  // autre variables
//--TODO--  bool notFinie=true;			// indique la fin de la décompression
//--TODO--  int posImage=0;				// pointeur dans l'image
//--TODO--
//--TODO--  // boucle de décodage
//--TODO--  while ( notFinie )
//--TODO--    {
//--TODO--      // lit le code courant
//--TODO--      curCode = lByte;
//--TODO--      while ( restBit<nbBit )		// tant qu'on n'a pas lu le nombre de bit correcte
//--TODO--   	{
//--TODO--	  // vérifie qu'il reste des données dans le buffer
//--TODO--	  if ( nbBuff<=0 )
//--TODO--	    {
//--TODO--	      nbBuff=cFichier[0];
//--TODO--	      cFichier++;
//--TODO--	    }
//--TODO--
//--TODO--	  // lit un octet du buffer décalé du nombre de bit restant dans lByte
//--TODO--	  // et incrémentation du pointeur
//--TODO--	  curCode |= cFichier[0]<<restBit;
//--TODO--	  cFichier++;
//--TODO--	  nbBuff--;					// un de moins
//--TODO--	  restBit+=8;					// 8 bit de plus de mit dans curCode
//--TODO--	}
//--TODO--      // nb de bit restant dans le dernier octet utilisé
//--TODO--      restBit-=nbBit;
//--TODO--      // lit le dernier octet et enleve les bits lu
//--TODO--      lByte=curCode>>nbBit;
//--TODO--      curCode&=maskBit;
//--TODO--
//--TODO--      if ( curCode==codeFin || curCode==codeClear )
//--TODO--   	{
//--TODO--	  if ( curCode==codeClear )
//--TODO--	    {		// code re-initialisation de l'alphabet
//--TODO--	      nbBit=tailleCode+1;
//--TODO--	      maxCode=1<<nbBit;
//--TODO--	      maskBit=0xFFFFFFFF>>(32-nbBit);
//--TODO--	      abPos=codeFin+1;
//--TODO--	      oldCode=-1;
//--TODO--	    }
//--TODO--	  if ( curCode==codeFin )
//--TODO--	    {		// code fin d'image
//--TODO--	      notFinie=false;
//--TODO--	    }
//--TODO--	}
//--TODO--      else
//--TODO--   	{ // code quelconque
//--TODO--	  tempCode=curCode;		// sauvegarde temporaire
//--TODO--	  // test si le code est valide
//--TODO--	  if ( curCode>abPos )
//--TODO--	    throw("Erreur de décompression");
//--TODO--	  // test si cas spécial
//--TODO--	  if ( curCode==abPos )
//--TODO--	    {
//--TODO--	      isCasSpecial=true;
//--TODO--	      curCode=oldCode;
//--TODO--	    }
//--TODO--	  while ( curCode>codeFin )
//--TODO--	    {		// decode le code
//--TODO--	      pileDecode[posPileDec++]=(uchar)abSuffx[curCode];	// lit le char decodé
//--TODO--	      curCode=abPrefx[curCode];						// lit le code suivant
//--TODO--	    }
//--TODO--	  tempPosPileDec=posPileDec;
//--TODO--	  // reste à mettre le code concret
//--TODO--	  pileDecode[posPileDec++]=(uchar)curCode;
//--TODO--	  // Maintenant, on deplie la pile pour la mettre dans l'image
//--TODO--	  while ( posPileDec!=0 )
//--TODO--	    image[posImage++]=pileDecode[--posPileDec];
//--TODO--	  // test si on doit ajouter une entrée dans l'alphabet
//--TODO--	  if ( isCasSpecial )
//--TODO--	    {
//--TODO--	      isCasSpecial=false;		// reset du cas special
//--TODO--	      // ajoute à l'image le premier char de la chaîne
//--TODO--	      image[posImage++]=pileDecode[tempPosPileDec];
//--TODO--	    }
//--TODO--	  if ( oldCode!=-1 )
//--TODO--	    {
//--TODO--	      // on prend pour suffx le premier char de la chaîne
//--TODO--	      // => le dernier de la pile de decodage
//--TODO--	      abSuffx[abPos]=pileDecode[tempPosPileDec];
//--TODO--	      abPrefx[abPos++]=oldCode;	  	// on le concaténe au code précedent
//--TODO--	      // test si on déborde le nombre de bits
//--TODO--	      if ( abPos==maxCode )
//--TODO--           	{
//--TODO--		  nbBit++;			// augmente le nombre de bits
//--TODO--		  maxCode=1<<nbBit;		// met à jour maxCode et maskBit
//--TODO--		  maskBit=0xFFFFFFFF>>(32-nbBit);
//--TODO--		}
//--TODO--	    }
//--TODO--	  oldCode=tempCode;		// met à jour oldCode
//--TODO--	}		// fin du if curCode==codeClear || curCode==codeFin
//--TODO--    }		// fin du while
//--TODO--}
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Convertie l'image 8 bits en 16 bits : 5-6-5
//--TODO--void Gif::Convert8to16(short int* dest)
//--TODO--{		// commence par convertir la palette
//--TODO--  short int tempPal[nbCouleur];			// palette temporaire 16 bits
//--TODO--  for (int i=0; i<nbCouleur; i++)
//--TODO-- tempPal[i]=(short int)(
//--TODO--   			  ((palette[i*3]>>3)<<11) |		// rouge
//--TODO--   			  ((palette[i*3+1]>>2)<<5) |		// vert
//--TODO--              (palette[i*3+2]>>3));				// bleu
//--TODO--	// converti l'image
//--TODO-- for (int i=0; i<imageW*imageH; i++)
//--TODO--   dest[i]=tempPal[image[i]];
//--TODO--}
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Convertie l'image 8 bits en 16 bits : 5-5-5
//--TODO--void Gif::Convert8to15(short int* dest)
//--TODO--{		// commence par convertir la palette
//--TODO--  short int tempPal[nbCouleur];			// palette temporaire 16 bits
//--TODO--  for (int i=0; i<nbCouleur; i++)
//--TODO--    tempPal[i]=(short int)(
//--TODO--			   ((palette[i*3]>>3)<<10) |		// rouge
//--TODO--			   ((palette[i*3+1]>>3)<<5) |		// vert
//--TODO--			   (palette[i*3+2]>>3));				// bleu
//--TODO--  // converti l'image
//--TODO--  for (int i=0; i<imageW*imageH; i++)
//--TODO--    dest[i]=tempPal[image[i]];
//--TODO--}
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Convertie l'image 8 bits en 24 bits
//--TODO--void Gif::Convert8to24(uchar* dest)
//--TODO--{
//--TODO--  for (int i=0; i<imageW*imageH; i++)
//--TODO--    {
//--TODO--      dest[i*3]=palette[image[i]*3];
//--TODO--      dest[i*3+1]=palette[image[i]*3+1];
//--TODO--      dest[i*3+2]=palette[image[i]*3+2];
//--TODO--    }
//--TODO--}
//--TODO--
//--TODO--//==============================================================================
//--TODO--// Convertie l'image 8 bits en 32 bits
//--TODO--void Gif::Convert8to32(int* dest)
//--TODO--{
//--TODO--  int tempPal[nbCouleur];		// palette temporaire 32 bits
//--TODO--  for (int i=0; i<nbCouleur; i++)
//--TODO--    tempPal[i]=(palette[i*3]<<16) |		// rouge
//--TODO--      (palette[i*3+1]<<8) |		// vert
//--TODO--      palette[i*3+2];				// bleu
//--TODO--  // converti l'image
//--TODO--  for (int i=0; i<imageW*imageH; i++)
//--TODO--    dest[i]=tempPal[image[i]];
//--TODO--}
