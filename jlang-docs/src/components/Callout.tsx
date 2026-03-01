import type { ReactNode } from 'react'

interface Props {
  type: 'note' | 'tip' | 'important' | 'warning' | 'life-lesson'
  children: ReactNode
}

const labels: Record<Props['type'], string> = {
  note: 'Note',
  tip: 'Tip',
  important: 'Important',
  warning: 'Warning',
  'life-lesson': 'Life Lesson',
}

export default function Callout({ type, children }: Props) {
  return (
    <div className={`callout callout--${type}`}>
      <div className="callout-title">{labels[type]}</div>
      <div className="callout-body">{children}</div>
    </div>
  )
}
