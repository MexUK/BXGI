#pragma once

#include "bxgi.h"

enum bxgi::EEvents
{
	// IMG
	UNSERIALIZE_IMG_ENTRY,
	SERIALIZE_IMG_ENTRY,
	ADD_IMG_ENTRY,
	REMOVE_IMG_ENTRY,
	ADD_IMG_ENTRY_FILE_EXTENSION,
	REMOVE_IMG_ENTRY_FILE_EXTENSION,
	REPLACE_IMG_ENTRY,
	REPLACE_IMG_ENTRY_SKIPPED_ENTRY,
	MERGE_IMG_ENTRY,
	SPLIT_IMG_ENTRY,
	EXPORT_IMG_ENTRY
};