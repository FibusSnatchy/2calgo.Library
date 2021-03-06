[Conditional("ObjectSet")] 
bool ObjectSet(Mq4String name, int index, Mq4Double value)
{
    _mq4ChartObjects.Set(name, index, value);
	return true;
}

[Conditional("ObjectGet")] 
Mq4Double ObjectGet(Mq4String name, int index)
{
    return _mq4ChartObjects.Get(name, index);
}
                
[Conditional("ObjectSetText")] 
bool ObjectSetText(Mq4String name, Mq4String text, int font_size = 11, string font = null, int color = CLR_NONE)
{
	_mq4ChartObjects.SetText(name, text, font_size, font, color);
	return true;
}

[Conditional("ObjectCreate")] 
bool ObjectCreate(Mq4String name, int type, int window, int time1, double price1, int time2 = 0, double price2 = 0, int time3 = 0, double price3 = 0)
{
    _mq4ChartObjects.Create(name, type, window, time1, price1, time2, price2, time3, price3);
	return true;
}
        
[Conditional("ObjectDelete")] 
bool ObjectDelete(Mq4String name)
{
    _mq4ChartObjects.Delete(name);
    return true;
}

[Conditional("ObjectFind")] 
int ObjectFind(Mq4String name)
{
    return _mq4ChartObjects.Find(name);
}

[Conditional("ObjectsDeleteAll")] 
int ObjectsDeleteAll(int window = EMPTY, int type=EMPTY)
{
    return _mq4ChartObjects.DeleteAll(type);
}
	
[Conditional("ObjectMove")] 
bool ObjectMove(Mq4String name, int point, int time, double price)
{
    _mq4ChartObjects.Move(name, point, time, price);
    return true;
}    

[Conditional("ObjectsTotal")] 
int ObjectsTotal(int type = EMPTY)
{
	return _mq4ChartObjects.ObjectsTotal(type);
}

[Conditional("ObjectName")] 
public string ObjectName(int index)
{
	return _mq4ChartObjects.ObjectName(index);		
}
	
[Conditional("ObjectType")] 
int ObjectType(Mq4String name)
{
	return _mq4ChartObjects.ObjectType(name);
}

[Conditional("ObjectSetFiboDescription")] 
bool ObjectSetFiboDescription(Mq4String name, int index, Mq4String text)
{
	_mq4ChartObjects.SetFiboDescription(name, index, text);
	return true;
}

[Conditional("ObjectGetFiboDescription")] 
Mq4String ObjectGetFiboDescription(Mq4String name, int index)
{
	return _mq4ChartObjects.GetFiboDescription(name, index);
}

[Conditional("ObjectGetValueByShift")] 
Mq4Double ObjectGetValueByShift(Mq4String name, int shift)
{
	return _mq4ChartObjects.ObjectGetValueByShift(name, shift);
}

[Conditional("ObjectDescription")] 
Mq4String ObjectDescription(Mq4String name)
{
	return _mq4ChartObjects.ObjectDescription(name);
}

