
elfResource* elfGetResource(elfList* resources, const char* name)
{
    elfResource* obj;

    for (obj = (elfResource*)elfBeginList(resources); obj; obj = (elfResource*)elfGetListNext(resources))
    {
        if (!strcmp(obj->name, name))
            return obj;
    }

    return NULL;
}

elfResource* elfGetResourceById(elfList* resources, int id)
{
    elfResource* obj;

    for (obj = (elfResource*)elfBeginList(resources); obj; obj = (elfResource*)elfGetListNext(resources))
    {
        if (obj->id == id)
            return obj;
    }

    return NULL;
}

void elfSetResourceUniqueName(elfList* namedObjects, elfResource* object)
{
    char* tname;
    char* nname;
    int dotPos;
    int num;

    if (object->name && strlen(object->name))
    {
        if (!elfGetResource(namedObjects, object->name))
        {
            return;
        }

        dotPos = elfRFindCharFromString('.', object->name);

        if (dotPos == (int)strlen(object->name) - 1)
        {
            tname = (char*)malloc(sizeof(char) * (strlen(object->name) + 1));
            memcpy(tname, object->name, sizeof(char) * strlen(object->name));
            tname[strlen(object->name)] = '\0';
        }
        else if (dotPos == -1 || !elfIsStringPositiveInt(&object->name[dotPos + 1]))
        {
            tname = (char*)malloc(sizeof(char) * (strlen(object->name) + 2));
            sprintf(tname, "%s.", object->name);
            tname[strlen(object->name) + 1] = '\0';
        }
        else
        {
            tname = (char*)malloc(sizeof(char) * (dotPos + 2));
            memcpy(tname, object->name, sizeof(char) * (dotPos + 1));
            tname[dotPos + 1] = '\0';
        }
    }
    else
    {
        tname = (char*)malloc(sizeof(char) * 8);
        memcpy(tname, "Object.", sizeof(char) * 7);
        tname[7] = '\0';
    }

    nname = (char*)malloc(sizeof(char) * (strlen(tname) + 12));
    memset(nname, 0x0, sizeof(char) * (strlen(tname) + 12));

    num = 1;
    sprintf(nname, "%s%d", tname, num);

    while (elfGetResource(namedObjects, nname))
    {
        memset(nname, 0x0, sizeof(char) * (strlen(tname) + 12));
        num++;
        sprintf(nname, "%s%d", tname, num);
        if (num < -1)
            break;
    }

    if (object->name)
        elfDestroyString(object->name);
    object->name = elfCreateString(nname);

    free(nname);
    free(tname);
}
