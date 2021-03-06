#include "pch.h"
#include "trace.h"

TRACELOGGING_DEFINE_PROVIDER(
    g_hProvider,
    "Microsoft.PowerToys",
    // {38e8889b-9731-53f5-e901-e8a7c1753074}
    (0x38e8889b, 0x9731, 0x53f5, 0xe9, 0x01, 0xe8, 0xa7, 0xc1, 0x75, 0x30, 0x74),
    TraceLoggingOptionProjectTelemetry());

struct ZoneSetInfo
{
    size_t NumberOfZones = 0;
    size_t NumberOfWindows = 0;
    ZoneSetLayout Layout = ZoneSetLayout::Custom;
};

ZoneSetInfo GetZoneSetInfo(_In_opt_ winrt::com_ptr<IZoneSet> set) noexcept
{
    ZoneSetInfo info;
    if (set)
    {
        auto zones = set->GetZones();
        info.NumberOfZones = zones.size();
        info.Layout = set->GetLayout();
        info.NumberOfWindows = std::count_if(zones.cbegin(), zones.cend(), [&](winrt::com_ptr<IZone> zone)
        {
            return !zone->IsEmpty();
        });
    }
    return info;
}


void Trace::RegisterProvider() noexcept
{
    TraceLoggingRegister(g_hProvider);
}

void Trace::UnregisterProvider() noexcept
{
    TraceLoggingUnregister(g_hProvider);
}

void Trace::FancyZones::EnableFancyZones(bool enabled) noexcept
{
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::EnableFancyZones",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingBoolean(enabled, "Enabled"));
}

void Trace::FancyZones::ToggleZoneViewers(bool visible) noexcept
{
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::ToggleZoneViewers",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingBoolean(visible, "Visible"));
}

void Trace::FancyZones::OnKeyDown(DWORD vkCode, bool win, bool control, bool inMoveSize) noexcept
{
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::OnKeyDown",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingValue(vkCode, "Hotkey"),
        TraceLoggingBoolean(win, "WindowsKey"),
        TraceLoggingBoolean(control, "ControlKey"),
        TraceLoggingBoolean(inMoveSize, "InMoveSize"));
}

void Trace::SettingsChanged(const Settings& settings) noexcept
{
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::SettingsChanged",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingBoolean(settings.shiftDrag, "Shift drag"),
        TraceLoggingBoolean(settings.displayChange_moveWindows, "Move Windows On Display Change"),
        TraceLoggingBoolean(settings.virtualDesktopChange_moveWindows, "Move Windows On Virtual Desktop Change"),
        TraceLoggingBoolean(settings.zoneSetChange_flashZones, "Flash zones On Zone Set Change"),
        TraceLoggingBoolean(settings.zoneSetChange_moveWindows, "Move Windows On Zone Set Change"),
        TraceLoggingBoolean(settings.overrideSnapHotkeys, "Override snap hot keys"),
        TraceLoggingWideString(settings.zoneHightlightColor.c_str(), "Zone highlight color"));
}

void Trace::VirtualDesktopChanged() noexcept
{
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::VirtualDesktopChanged",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE));
}

void Trace::ZoneWindow::KeyUp(WPARAM wParam, bool isEditorMode) noexcept
{
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::ZoneWindowKeyUp",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingValue(wParam, "Keyboard value"),
        TraceLoggingBoolean(isEditorMode, "Editor Mode"));
}

void Trace::ZoneWindow::MoveSizeEnd(_In_opt_ winrt::com_ptr<IZoneSet> activeSet) noexcept
{
    auto const zoneInfo = GetZoneSetInfo(activeSet);
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::MoveSizeEnd",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingValue(reinterpret_cast<void*>(activeSet.get()), "Active Set"),
        TraceLoggingValue(zoneInfo.NumberOfZones, "NumberOfZones"),
        TraceLoggingValue(zoneInfo.NumberOfWindows, "NumberOfWindows"),
        TraceLoggingValue(static_cast<int>(zoneInfo.Layout), "LayoutKind"));
}

void Trace::ZoneWindow::CycleActiveZoneSet(_In_opt_ winrt::com_ptr<IZoneSet> activeSet, InputMode mode) noexcept
{
    auto const zoneInfo = GetZoneSetInfo(activeSet);
    TraceLoggingWrite(
        g_hProvider,
        "FancyZones::Event::CycleActiveZoneSet",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingKeyword(PROJECT_KEYWORD_MEASURE),
        TraceLoggingValue(reinterpret_cast<void*>(activeSet.get()), "Active Set"),
        TraceLoggingValue(zoneInfo.NumberOfZones, "NumberOfZones"),
        TraceLoggingValue(zoneInfo.NumberOfWindows, "NumberOfWindows"),
        TraceLoggingValue(static_cast<int>(zoneInfo.Layout), "LayoutKind"),
        TraceLoggingValue(static_cast<int>(mode), "InputMode"));
}

void Trace::ZoneWindow::EditorModeActivity::Start() noexcept
{
    m_activity = TraceLoggingActivity<g_hProvider, PROJECT_KEYWORD_MEASURE>();
    TraceLoggingWriteStart(
        m_activity.value(),
        "FancyZones::Activity::EditorMode",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance));
}

void Trace::ZoneWindow::EditorModeActivity::Stop(_In_opt_ winrt::com_ptr<IZoneSet> activeSet) noexcept
{
    auto const zoneInfo = GetZoneSetInfo(activeSet);
    TraceLoggingWriteStop(
        m_activity.value(),
        "FancyZones::Activity::EditorMode",
        ProjectTelemetryPrivacyDataTag(ProjectTelemetryTag_ProductAndServicePerformance),
        TraceLoggingValue(reinterpret_cast<void*>(activeSet.get()), "Active Set"),
        TraceLoggingValue(zoneInfo.NumberOfZones, "NumberOfZones"),
        TraceLoggingValue(zoneInfo.NumberOfWindows, "NumberOfWindows"),
        TraceLoggingValue(static_cast<int>(zoneInfo.Layout), "LayoutKind"));
    m_activity.reset();
}
