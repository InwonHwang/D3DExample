#pragma once

class ResourceItem
{
public:
	enum State {
		CREATED,
		LOADED,
		DISABLED
	};

public:
	ResourceItem() {}
	~ResourceItem() {}

	virtual void create() = 0;
	virtual void destroy() = 0;
	virtual void disable() = 0;
	virtual void restore() = 0;

	bool isInitialized() const;
	bool isLoaded() const;
	bool isDisabled() const;

protected:
	void notifyCreated();
	void notifyDestroyed();
	void notifyLoaded();
	void notifyUnloaded();
	void notifyRestored();
	void notifyDisabled();

private:
	flags32 _stateFlags;
};

inline bool ResourceItem::isInitialized() const
{
	return _stateFlags.check(CREATED);
}

inline bool ResourceItem::isLoaded() const
{
	return _stateFlags.check(LOADED);
}

inline bool ResourceItem::isDisabled() const
{
	return _stateFlags.check(DISABLED);
}

inline void ResourceItem::notifyCreated()
{
	_stateFlags.set(CREATED);
}

inline void ResourceItem::notifyDestroyed()
{
	_stateFlags.reset(CREATED);
}

inline void ResourceItem::notifyLoaded()
{
	_stateFlags.set(LOADED);
}

inline void ResourceItem::notifyUnloaded()
{
	_stateFlags.reset(LOADED);
}

inline void ResourceItem::notifyDisabled()
{
	_stateFlags.set(DISABLED);
}

inline void ResourceItem::notifyRestored()
{
	_stateFlags.reset(DISABLED);
}


