#include "engine/plugin_manager.h"
#include "engine/iplugin.h"
#include "core/log.h"
#include "core/pod_array.h"
#include <Windows.h>


namespace Lux 
{
	struct PluginManagerImpl
	{
		typedef PODArray<IPlugin*> PluginList;

		Engine* m_engine;
		PluginList m_plugins;
	};


	void PluginManager::update(float dt)
	{
		PluginManagerImpl::PluginList& plugins = m_impl->m_plugins;
		for(int i = 0, c = plugins.size(); i < c; ++i)
		{
			plugins[i]->update(dt);
		}
	}


	void PluginManager::serialize(ISerializer& serializer)
	{
		PluginManagerImpl::PluginList& plugins = m_impl->m_plugins;
		for(int i = 0, c = plugins.size(); i < c; ++i)
		{
			plugins[i]->serialize(serializer);
		}
	}


	void PluginManager::deserialize(ISerializer& serializer)
	{
		PluginManagerImpl::PluginList& plugins = m_impl->m_plugins;
		for(int i = 0, c = plugins.size(); i < c; ++i)
		{
			plugins[i]->deserialize(serializer);
		}
	}


	void PluginManager::onDestroyUniverse(Universe& universe)
	{
		PluginManagerImpl::PluginList& plugins = m_impl->m_plugins;
		for(int i = 0, c = plugins.size(); i < c; ++i)
		{
			plugins[i]->onDestroyUniverse(universe);
		}
	}


	void PluginManager::onCreateUniverse(Universe& universe)
	{
		PluginManagerImpl::PluginList& plugins = m_impl->m_plugins;
		for(int i = 0, c = plugins.size(); i < c; ++i)
		{
			plugins[i]->onCreateUniverse(universe);
		}
	}

	IPlugin* PluginManager::getPlugin(const char* name)
	{
		for(int i = 0; i < m_impl->m_plugins.size(); ++i)
		{
			if(strcmp(m_impl->m_plugins[i]->getName(), name) == 0)
			{
				return m_impl->m_plugins[i];
			}
		}
		return 0;
	}

	IPlugin* PluginManager::load(const char* path)
	{
		g_log_info.log("plugins", "loading plugin %s", path);
		typedef IPlugin* (*PluginCreator)();
		HMODULE lib = LoadLibrary(TEXT(path));
		if(lib)
		{
			PluginCreator creator = (PluginCreator)GetProcAddress(lib, TEXT("createPlugin"));
			if(creator)
			{
				IPlugin* plugin = creator();
				if(!plugin->create(*m_impl->m_engine))
				{
					LUX_DELETE(plugin);
					ASSERT(false);
					return false;
				}
				m_impl->m_plugins.push(plugin);
				g_log_info.log("plugins", "plugin laoded");
				return plugin;
			}
		}
		return 0;
	}


	void PluginManager::addPlugin(IPlugin* plugin)
	{
		m_impl->m_plugins.push(plugin);
	}

	
	bool PluginManager::create(Engine& engine)
	{
		m_impl = LUX_NEW(PluginManagerImpl)();
		m_impl->m_engine = &engine;
		return true;
	}


	void PluginManager::destroy()
	{
		for(int i = 0; i < m_impl->m_plugins.size(); ++i)
		{
			m_impl->m_plugins[i]->destroy();
			LUX_DELETE(m_impl->m_plugins[i]);
		}
		LUX_DELETE(m_impl);
		m_impl = NULL;
	}


} // ~namespace Lux